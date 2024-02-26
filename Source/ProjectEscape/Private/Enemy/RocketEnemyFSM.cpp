// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RocketEnemyFSM.h"

#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/Rocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

URocketEnemyFSM::URocketEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick=true;

	AttackDistance = 3000;

	MinAttackTime = 4.0f;
	MaxAttackTime = 6.0f;

}

void URocketEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();
	AttackTime = FMath::RandRange( MinAttackTime, MaxAttackTime );
	FVector MuzzleLoc = Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

	if ( CurrentTime > AttackTime )
	{
		CurrentTime = 0;
		//EnemyAnim->PlayShootMontage();

		FVector DirectionToPlayer = (Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		FRotator RotationToPlayer = DirectionToPlayer.Rotation();

		check( EnemyRocketFactory );
		GetWorld()->SpawnActor<ARocket>( EnemyRocketFactory, MuzzleLoc, RotationToPlayer );

		//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), MuzzleFlash, MuzzleLoc, FRotator(), FVector( 1 ), true );
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), AttackSound, MuzzleLoc );
	}

	float dist = FVector::Dist( Player->GetActorLocation(), Enemy->GetActorLocation() );
	// 그 거리가 AttackDistance를 초과한다면
	if ( dist > AttackDistance || bCanSeePlayer == false ) {
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
		//EnemyAnim->IsAttack = false;
	}
}

void URocketEnemyFSM::FireRocket()
{

}
