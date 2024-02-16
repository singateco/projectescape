// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatsComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "Player/Upgrade.h"
#include "ProjectEscape/PEGameplayTags.h"


// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
}

float UPlayerStatsComponent::GetGunDamage() const
{
	return GunDamage * GunDamageMultiplier;
}

void UPlayerStatsComponent::InitializeStat()
{
	GunDamage = InitialGunDamage;
	GunDamageMultiplier = InitialGunDamageMultiplier;
}


void UPlayerStatsComponent::CalculateStat()
{
	
	InitializeStat();
	
	for (UUpgrade* Upgrade : OwningUpgrades)
	{
		for (TTuple<FGameplayTag, float> Mod : Upgrade->StaticData.StatsToAlwaysModify)
		{
			if (Mod.Key.MatchesTagExact(PEGameplayTags::Stat_Gun_Damage))
			{
				GunDamage += Mod.Value;
			}

			if (Mod.Key.MatchesTagExact(PEGameplayTags::Stat_Gun_DamageMultiplier))
			{
				GunDamageMultiplier += Mod.Value;
			}
		}
	}
}

void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialGunDamage = GunDamage;
	InitialGunDamageMultiplier = GunDamageMultiplier;

	Player = GetOwner<AProjectEscapePlayer>();

	TArray<FUpgradeData*> OutData;
	UpgradeDataTable->GetAllRows(TEXT("Context"), OutData);

	UUpgrade* Upgrade = NewObject<UUpgrade>(this, UUpgrade::StaticClass(), TEXT("Upgrade 1"));
	Upgrade->StaticData = *OutData[0];
	this->OwningUpgrades.Add(Upgrade);
	CalculateStat();
}
