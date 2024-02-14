// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Navigation/PathFollowingComponent.h"

#include "Player/ProjectEscapePlayer.h"

// Sets default values for this component's properties
UEnemyBaseFSM::UEnemyBaseFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEnemyBaseFSM::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemyBase>(GetOwner());
	EnemyAnim = Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	Ai = Cast<AAIController>(Enemy->GetController());
	Player = Cast<AProjectEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	HP = 0;
	UpdateHP(MaxHP);
	
}


// Called every frame
void UEnemyBaseFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::Idle: TickIdle(); break;
	case EEnemyState::Move: TickMove(); break;
	case EEnemyState::Attack: TickAttack(); break;
	case EEnemyState::Damage: TickDamage(); break;
	case EEnemyState::Die: TickDie(); break;
	}
}

void UEnemyBaseFSM::SetState(EEnemyState Next)
{
	//check(EnemyAnim);
	// 이동상태로 전이한다면
	if (Next == EEnemyState::Move)
	{
		// 랜덤위치를 갱신하고싶다.
		UpdateRandomLocation(Enemy->GetActorLocation(), 500, RandomLocation);
	}

	State = Next;
	//EnemyAnim->State = Next;
	
}

void UEnemyBaseFSM::TickIdle()
{
	if (Player)
	{
		SetState(EEnemyState::Move);
	}
}

void UEnemyBaseFSM::TickMove()
{
	FVector destination = Player->GetActorLocation();
	// 목적지를 향해서 이동하고싶다.
	FVector dir = destination - Enemy->GetActorLocation();

	// 타겟이 길 위에 있다면
	auto ns= UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(50);
	req.SetGoalLocation(destination);
	Ai->BuildPathfindingQuery(req, query);
	auto result = ns->FindPathSync(query);
	if (result.IsSuccessful())
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
			UpdateRandomLocation(Enemy->GetActorLocation(), 500, RandomLocation );
		}
	}

	if (Player)
	Ai->SetFocus(Player);

	if(dir.Size() <= AttackDistance && bCanSeePlayer )
	{
		SetState(EEnemyState::Attack);
	}
}

void UEnemyBaseFSM::TickAttack()
{
	//float dist = FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation());
	//// 그 거리가 AttackDistance를 초과한다면
	//if (dist > AttackDistance) {
	//	// 이동상태로 전이하고싶다.
	//	SetState(EEnemyState::Move);
	//	//EnemyAnim->IsAttack = false;
	//}
}

void UEnemyBaseFSM::TickDamage()
{

}

void UEnemyBaseFSM::TickDie()
{
	// 죽으면 총 안맞게 하기	
	Enemy->GetCapsuleComponent()->SetCollisionResponseToChannel( ECC_Visibility, ECR_Ignore );

	CurrentTime += GetWorld()->GetDeltaSeconds();

	if(CurrentTime > WidgetTime )
	{
		Enemy->EnemyHPComponent->SetVisibility( false );
	}
	
	if(CurrentTime > DieTime )
	{
		Enemy->Destroy();
	}

}

void UEnemyBaseFSM::OnTakeDamage(int32 Damage)
{
	//Ai->StopMovement();
	UpdateHP(-Damage);
	if(HP > 0)
	{
		//SetState(EEnemyState::Damage);
		EnemyAnim->PlayHitAnimMontage();
	}
	else
	{
		SetState( EEnemyState::Die );
		EnemyAnim->PlayDieAnimMontage();
	}
}

void UEnemyBaseFSM::UpdateHP(int32 NewHP)
{
	HP = FMath::Max(0, HP + NewHP);

	Enemy->DoDamageUpdateUI( HP, MaxHP );
}

bool UEnemyBaseFSM::UpdateRandomLocation(FVector origin, float radius, FVector& outLocation)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem( GetWorld() );
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius( origin, radius, loc );
	if ( result )
	{
		outLocation = loc.Location;
		return true;
	}
	return false;
}


