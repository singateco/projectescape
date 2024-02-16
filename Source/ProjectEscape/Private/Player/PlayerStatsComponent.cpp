// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatsComponent.h"

#include "Character/Effect.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyStatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/FireComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "Player/Upgrade.h"
#include "ProjectEscape/PEGameplayTags.h"


// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
}

float UPlayerStatsComponent::GetGunDamage(const bool bHitWeakPoint) const
{
	return GunDamage * GunDamageMultiplier * bHitWeakPoint ? GunWeakPointMultiplier : 1.f;
}

void UPlayerStatsComponent::OnEnemyHitByPlayerGun(AEnemyBase* Enemy, FHitResult HitResult)
{
	UBoxComponent* WeakPointBox = Enemy->WeakPoint;
	bool bHitWeakPoint = false;
	if (WeakPointBox)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TEnumAsByte<EObjectTypeQuery> Weakpoint = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4);
		ObjectTypes.Add(Weakpoint);
		TArray<AActor*> ActorsToIgnore;
		TArray<UPrimitiveComponent*> OutComponents;
		
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 12.f, 16, FColor::White, false, 2, 0, 1);
		//DrawDebugBox(GetWorld(), WeakPointBox->GetComponentLocation(), WeakPointBox->GetScaledBoxExtent(), FColor::Red, false, 2.f, 0, 2.f);

		bHitWeakPoint = UKismetSystemLibrary::SphereOverlapComponents(
			GetWorld(),
			HitResult.ImpactPoint,
			12.f,
			ObjectTypes,
			nullptr,
			ActorsToIgnore,
			OutComponents);
	}
	
	Enemy->ProcessDamage(GetGunDamage(bHitWeakPoint));
	for (TSubclassOf<UEffect> EffectBPClass : EffectsToApplyWhenEnemyHitByPlayerGun)
	{
		UEffect* NewEffect = NewObject<UEffect>(Enemy->EnemyStatsComponent, EffectBPClass);
		Enemy->EnemyStatsComponent->AddEffect(NewEffect);
	}
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
	Player->FireComponent->OnEnemyHitByPlayerGun.AddUniqueDynamic(this, &UPlayerStatsComponent::OnEnemyHitByPlayerGun);
}
