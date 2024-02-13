// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseFSM.h"

#include "AIController.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
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
	//if (Next == EEnemyState::Move)
	//{
	//	// 랜덤위치를 갱신하고싶다.
	//	UpdateRandomLocation(Me->GetActorLocation(), 500, RandomLocation);
	//}

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

	Enemy->AddMovementInput(dir.GetSafeNormal());

	if (Player)
	Ai->SetFocus(Player);

	if(dir.Size() <= AttackDistance && Enemy->bCanSeePlayer == true)
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

}

void UEnemyBaseFSM::OnTakeDamage(int32 Damage)
{
	Ai->StopMovement();
	UpdateHP(-Damage);
	if(HP > 0)
	{
		SetState(EEnemyState::Damage);
		//PlayMontageDamage();
	}
}

void UEnemyBaseFSM::UpdateHP(int32 NewHP)
{
	HP = FMath::Max(0, HP + NewHP);
}

//void UEnemyBaseFSM::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
//{
//	for (AActor* Actor : UpdatedActors)
//	{
//		// �÷��̾ �����Ǿ��� ��
//		if (Actor->IsA<AProjectEscapePlayer>())
//		{
//			SetState(EEnemyState::Move); // �̵� ���·� ����
//			return;
//		}
//	}
//	// ������ �÷��̾ ���� ��
//	SetState(EEnemyState::Idle); // ��� ���·� ����
//}
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

