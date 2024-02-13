// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBaseFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Enemy/EnemyAIPerception.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/ProjectEscapePlayer.h"

// Sets default values for this component's properties
UEnemyBaseFSM::UEnemyBaseFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	EnemyAIPerception = CreateDefaultSubobject<UEnemyAIPerception>(TEXT("EnemyAIperception"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 3200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;

	EnemyAIPerception->ConfigureSense(*SightConfig);




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
	if (Player)
	{
		SetState(EEnemyState::Move);
	}
}

void UEnemyBaseFSM::TickMove()
{
	FVector destination = Player->GetActorLocation();
	// �������� ���ؼ� �̵��ϰ�ʹ�.
	FVector dir = destination - Enemy->GetActorLocation();

	Enemy->AddMovementInput(dir.GetSafeNormal());

	if (Player)
	Ai->SetFocus(Player);

	if(dir.Size() <= AttackDistance)
	{
		SetState(EEnemyState::Attack);
	}
}

void UEnemyBaseFSM::TickAttack()
{


	//float dist = FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation());
	//// �� �Ÿ��� AttackDistance�� �ʰ��Ѵٸ�
	//if (dist > AttackDistance) {
	//	// �̵����·� �����ϰ�ʹ�.
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
	if(HP>0)
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

