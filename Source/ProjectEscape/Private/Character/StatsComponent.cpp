// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StatsComponent.h"


// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize HP.
	HP = MaxHP;
}

void UStatsComponent::ProcessDamage(const float DamageValue)
{
	HP = FMath::Max(HP - DamageValue, 0);
	
	if (HP <= 0)
	{
		ProcessDying();
		return;
	}

	OnTakenDamage.Broadcast();
	OnHPChanged.Broadcast(MaxHP, HP);
}
