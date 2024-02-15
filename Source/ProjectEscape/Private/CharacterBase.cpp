// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectEscape/Public/CharacterBase.h"

#include "PECharacterMovementComponent.h"
#include "Character/StatsComponent.h"

ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPECharacterMovementComponent>(
		  ACharacter::CharacterMovementComponentName)),
	StatsComponent(CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component")))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (MaxHP != -1)
	{
		StatsComponent->SetMaxHP(MaxHP);
	}
}