// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerStatsComponent.h"
#include "Character/Effect.h"
#include "Components/BoxComponent.h"
#include "Components/TextBlock.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyStatsComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/FireComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "Player/Upgrade.h"
#include "ProjectEscape/PEGameplayTags.h"
#include "System/ProjectEscapePlayerController.h"
#include "UI/MainUI.h"


// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
}

float UPlayerStatsComponent::GetGunDamage(const bool bHitWeakPoint) const
{
	float Damage = GunDamage * GunDamageMultiplier;
	if (bHitWeakPoint)
	{
		Damage *= GunWeakPointMultiplier;
	}
	return Damage;
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
		
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 14.f, 32, FColor::White, false, 2, 0, 1);
		//DrawDebugBox(GetWorld(), WeakPointBox->GetComponentLocation(), WeakPointBox->GetScaledBoxExtent(), FColor::Red, false, 2.f, 0, 2.f);

		bHitWeakPoint = UKismetSystemLibrary::SphereOverlapComponents(
			GetWorld(),
			HitResult.ImpactPoint,
			14.f,
			ObjectTypes,
			nullptr,
			ActorsToIgnore,
			OutComponents);
	}

	const float CalculatedGunDamage = GetGunDamage(bHitWeakPoint);
	Enemy->ProcessDamage(CalculatedGunDamage);
	const bool IsDyingHit = Enemy->GetStatsComponent()->GetHP() <= 0;

	if (Player->FireComponent->MainUI)
	{
		Player->FireComponent->MainUI->ShowEnemyHit(IsDyingHit);
	}
	
	for (TSubclassOf<UEffect> EffectBPClass : EffectsToApplyWhenEnemyHitByPlayerGun)
	{
		UEffect* NewEffect = NewObject<UEffect>(Enemy->EnemyStatsComponent, EffectBPClass);
		Enemy->EnemyStatsComponent->AddEffect(NewEffect);
	}
}

float UPlayerStatsComponent::GetStat(FGameplayTag GameplayTag)
{
	if (float** Found = StatMap.Find(GameplayTag))
	{
		return **Found;
	}

	return 0;
}

void UPlayerStatsComponent::UpdateStat(FGameplayTag GameplayTag, const float NewValue)
{
	if (float** Ptr = StatMap.Find(GameplayTag))
	{
		**Ptr = NewValue;
	}
}

void UPlayerStatsComponent::AddUpgrade(UUpgrade* Upgrade)
{
	OwningUpgrades.Add(Upgrade);
	if (const TArray<TSubclassOf<UEffect>> GunEffects = Upgrade->StaticData.GunEffects; !GunEffects.IsEmpty())
	{
		EffectsToApplyWhenEnemyHitByPlayerGun.Append(GunEffects);
	}

	if (const TArray<TSubclassOf<UEffect>> PlayerEffects = Upgrade->StaticData.PlayerEffects; !PlayerEffects.IsEmpty())
	{
		for (TSubclassOf<UEffect> EffectClass : PlayerEffects)
		{
			AddEffect(NewObject<UEffect>(this, EffectClass));
		}
	}
}


void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentBullets = MaxBullets;

	Player = GetOwner<AProjectEscapePlayer>();
	Player->FireComponent->OnEnemyHitByPlayerGun.AddUniqueDynamic(this, &UPlayerStatsComponent::OnEnemyHitByPlayerGun);
}
