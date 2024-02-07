// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RifleEnemyFSM.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/RifleEnemy.h"
#include "Player/ProjectEscapePlayer.h"


#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
URifleEnemyFSM::URifleEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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


