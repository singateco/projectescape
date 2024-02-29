// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

#include "Components/WidgetComponent.h"
#include "AI/BossAIController.h"
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
		EnemyHPComponent->SetHiddenInGame( true );

	}

	ThrowRef = CreateDefaultSubobject<USceneComponent>( TEXT( "ThrowRef" ) );
	ThrowRef->SetupAttachment( RootComponent );

	MaxHP = 100;

	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	AttachPistol();
	EnemyMaxSpeed = 500.f;
}

UBehaviorTree* ABossEnemy::GetBehaviorTree()
{
	return Tree;
}

void ABossEnemy::AttachPistol()
{
	TArray<UActorComponent*> Comp = this->GetComponentsByTag( USkeletalMeshComponent::StaticClass(), TEXT( "Body" ) );
	USceneComponent* BodyComp = Cast<USceneComponent>( Comp[0] );
	GunMesh->AttachToComponent( BodyComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT( "GunPosition" ) );
}


