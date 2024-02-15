// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatsComponent.h"


// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
}

float UPlayerStatsComponent::GetGunDamage()
{
	return GunDamage * GunDamageMultiplier;
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}
