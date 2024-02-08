// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RifleEnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/ArrowComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/RifleEnemy.h"
#include "Player/ProjectEscapePlayer.h"


#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
URifleEnemyFSM::URifleEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AttackDistance = 1000;
}


// Called when the game starts
void URifleEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void URifleEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URifleEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

	if(CurrentTime > AttackTime)
	{
		CurrentTime = 0;
		//EnemyAnim->IsAttack = true;

		GetWorld()->SpawnActor<AEnemyBullet>(AEnemyBullet::StaticClass(), Enemy->BulletREF->GetComponentTransform());
	}

	float dist = FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation());
	// 그 거리가 AttackDistance를 초과한다면
	if (dist > AttackDistance) {
		// 이동상태로 전이하고싶다.
		SetState(EEnemyState::Move);
		//EnemyAnim->IsAttack = false;
	}
}


