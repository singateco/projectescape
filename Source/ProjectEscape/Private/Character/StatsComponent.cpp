// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/StatsComponent.h"

#include "CharacterBase.h"
#include "Character/Effect.h"
#include "ProjectEscape/PEGameplayTags.h"


// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningChara = GetOwner<ACharacterBase>();
	
	// Initialize HP.
	SetHP(MaxHP);
}

void UStatsComponent::AddEffect(UEffect* EffectToAdd)
{
	EffectToAdd->EffectOwner = OwningChara;
	EffectToAdd->OwningStatsComponent = this;
	
	Effects.Add(EffectToAdd);
	EffectToAdd->OnEffectEnded.AddUniqueDynamic(this, &UStatsComponent::RemoveEffect);

	EffectToAdd->Initialize();
}

void UStatsComponent::RemoveEffect(UEffect* EffectToRemove)
{
	Effects.RemoveSwap(EffectToRemove);

	if (!EffectToRemove) return;
	if (!EffectToRemove->IsValidLowLevel()) return;
	EffectToRemove->ConditionalBeginDestroy();
	EffectToRemove = nullptr;
}

void UStatsComponent::ProcessDamage(const float DamageValue)
{
	if (OwningChara->HasMatchingGameplayTag(PEGameplayTags::Status_IsDead))
	{
		return;
	}
	
	HP = FMath::Max(HP - DamageValue, 0);
	OnHPChanged.Broadcast(MaxHP, HP);
	OnTakenDamage.Broadcast(DamageValue);
	
	if (HP <= 0)
	{
		ProcessDying();
	}
}

void UStatsComponent::ProcessDying()
{
	AddTag(PEGameplayTags::Status_IsDead);
	OnHPReachedZero.Broadcast();
}
