// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
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

	// ...
}


// Called when the game starts
void UEnemyBaseFSM::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemyBase>(GetOwner());

	EnemyAnim = Cast<UEnemyAnimInstance>(Enemy->GetMesh()->GetAnimInstance());

	Ai = Cast<AAIController>(Enemy->GetController());

	
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
	// �̵����·� �����Ѵٸ�
	//if (Next == EEnemyState::Move)
	//{
	//	// ������ġ�� �����ϰ�ʹ�.
	//	UpdateRandomLocation(Me->GetActorLocation(), 500, RandomLocation);
	//}

	State = Next;
	//EnemyAnim->State = Next;
	
}

void UEnemyBaseFSM::TickIdle()
{
	Player = Cast<AProjectEscapePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		SetState(EEnemyState::Move); // ����
	}
}

void UEnemyBaseFSM::TickMove()
{
	FVector destination = Player->GetActorLocation();
	// �������� ���ؼ� �̵��ϰ�ʹ�.
	FVector dir = destination - Enemy->GetActorLocation();

	Enemy->AddMovementInput(dir.GetSafeNormal());

	if(dir.Size() <= AttackDistance)
	{
		SetState(EEnemyState::Attack);
	}

}

void UEnemyBaseFSM::TickAttack()
{

}

void UEnemyBaseFSM::TickDamage()
{

}

void UEnemyBaseFSM::TickDie()
{

}

//bool UEnemyBaseFSM::UpdateRandomLocation(FVector origin, float radius, FVector& outLocation)
//{
//	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	FNavLocation loc;
//	bool result = ns->GetRandomReachablePointInRadius(origin, radius, loc);
//	if (result)
//	{
//		outLocation = loc.Location;
//		return true;
//	}
//
//	return false;
//}

