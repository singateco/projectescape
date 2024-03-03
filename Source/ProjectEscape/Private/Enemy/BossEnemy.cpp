// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

#include "Components/WidgetComponent.h"
#include "AI/BossAIController.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBaseFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	PlaySpawnEffect = false;

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
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

void ABossEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetStatsComponent()->GetHP() <= 0 )
	{
		OnDie();
	}
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

void ABossEnemy::OnDie()
{
	GetCapsuleComponent()->SetCollisionProfileName( FName( "NoCollision" ) );
	GetMesh()->SetCollisionProfileName( TEXT( "Ragdoll" ) );
	GetMesh()->SetSimulatePhysics( true );

}


