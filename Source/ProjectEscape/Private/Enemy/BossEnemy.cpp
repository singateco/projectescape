// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

#include "Components/WidgetComponent.h"
#include "Enemy/BossAIController.h"
#include "Enemy/EnemyBaseFSM.h"

ABossEnemy::ABossEnemy( const FObjectInitializer& ObjectInitializer )
	:
	Super( ObjectInitializer )

{
	PrimaryActorTick.bCanEverTick=true;

	if (EnemyBaseFSM)
	{
		EnemyBaseFSM->DestroyComponent();
		EnemyBaseFSM->SetActive( false );
	}

	if(EnemyHPComponent)
	{
		EnemyHPComponent->SetHiddenInGame(true);
	}

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	
}
