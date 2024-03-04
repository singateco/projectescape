// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "AI/BossAIController.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyBaseFSM.h"
#include "NiagaraSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/HealthPickup.h"

ABossEnemy::ABossEnemy( const FObjectInitializer& ObjectInitializer )
	:
	Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick=true;

	BossAura = CreateDefaultSubobject<UNiagaraComponent>( TEXT( "BossAura" ) );
	BossAura->SetupAttachment( GetCapsuleComponent() );
	BossAura->SetRelativeLocation( FVector( 0, 0, 50 ) );

	static const ConstructorHelpers::FObjectFinder<UNiagaraSystem> BossAuraFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/VFX_Pickup_Pack_1/VFX/Presets/NE_VFX_Loot_Idle_03.NE_VFX_Loot_Idle_03'" ) };

	if ( BossAuraFinder.Succeeded() )
	{
		BossAura->SetAsset( BossAuraFinder.Object );
	}

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

	//EnemyName=TEXT( "Lyla" );

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

void ABossEnemy::ProcessDying()
{
	Super::ProcessDying();
	GetCapsuleComponent()->SetCollisionProfileName( FName( "NoCollision" ) );
	GetMesh()->SetCollisionProfileName( TEXT( "Ragdoll" ) );
	GetMesh()->SetSimulatePhysics( true );
	BossAura->SetVisibility( false );
	IsDead = true;
}

bool ABossEnemy::GetIsDead()
{
	return IsDead;
}

