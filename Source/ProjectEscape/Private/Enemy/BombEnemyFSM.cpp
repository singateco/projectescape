// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombEnemyFSM.h"
#include "AIController.h"

#include "AITypes.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Player/ProjectEscapePlayer.h"

UBombEnemyFSM::UBombEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick=true;

	AttackDistance = 600.0f;
	AttackTime = 0.5f;
}

void UBombEnemyFSM::TickMove()
{
	FVector destination=Player->GetActorLocation();
	// 목적지를 향해서 이동하고싶다.
	FVector dir=destination - Enemy->GetActorLocation();

	// 타겟이 길 위에 있다면
	auto ns=UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius( 50 );
	req.SetGoalLocation( destination );
	Ai->BuildPathfindingQuery( req, query );
	auto result=ns->FindPathSync( query );
	if ( result.IsSuccessful() )
	{
		//  타겟을 향해 이동하고싶다.
		Ai->MoveToLocation( destination );
	}
	else // 그렇지 않다면
	{
		//  길 위에 랜덤한 위치를 하나 정해서 그곳으로 이동하고싶다.
		FPathFollowingRequestResult r;
		r.Code=Ai->MoveToLocation( RandomLocation );
		//  만약 그곳에 도착했거나 문제가 있다면 랜덤한 위치를 갱신하고싶다.
		if ( r != EPathFollowingRequestResult::RequestSuccessful )
		{
			UpdateRandomLocation( Enemy->GetActorLocation(), 500, RandomLocation );
		}
	}

	if ( dir.Size() <= AttackDistance && bCanSeePlayer )
	{
		SetState( EEnemyState::Attack );
	}
}

void UBombEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();
	if(CurrentTime > AttackTime )
	{
		Explosion();
	}

	float dist=FVector::Dist( Player->GetActorLocation(), Enemy->GetActorLocation() );
	// 그 거리가 AttackDistance를 초과한다면
	if ( dist > AttackDistance || bCanSeePlayer == false ) {
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
	}
}

void UBombEnemyFSM::TickDie()
{
	Super::TickDie();
	Explosion();
}

void UBombEnemyFSM::OnTakeDamage(float Damage_Unused)
{
	
}

void UBombEnemyFSM::Explosion()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnoreArray;
	ActorsToIgnoreArray.Add( Enemy );
	TArray<AActor*> OutActorsArray;

	FVector EnemyLoc = Enemy->GetActorLocation();

	//DrawDebugSphere( GetWorld(), RocketMesh->GetComponentLocation(), SphereRadius, 50, FColor::Red, false, 3 );

	bool bSphereOverlapResult=UKismetSystemLibrary::SphereOverlapActors( GetWorld(), EnemyLoc, SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter = Cast<ACharacterBase>( HitActor );
			if ( OtherCharacter )
			{
				FVector StartLoc=EnemyLoc;
				FVector EndLoc=OtherCharacter->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor( Enemy );
				Params.AddIgnoredActor( HitActor );

				bool bHit=GetWorld()->LineTraceSingleByChannel( HitResult, StartLoc, EndLoc, ECC_Visibility, Params );
				// 라인 트레이스에서 플레이어가 충돌한 경우 데미지를 입힘
				if ( !bHit )
				{
					OtherCharacter->ProcessDamageFromLoc( 5, HitResult );
				}
			}
		}
	}

	//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, RocketLoc, FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, EnemyLoc, FRotator(), FVector( 1 ), true );
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSound, EnemyLoc );

	Enemy->OnEnemyDied.Broadcast(Enemy);
	Enemy->Destroy();
}
