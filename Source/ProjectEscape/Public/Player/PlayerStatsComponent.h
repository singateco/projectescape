// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/StatsComponent.h"
#include "ProjectEscape/PEGameplayTags.h"
#include "PlayerStatsComponent.generated.h"


class AEnemyBase;
class AProjectEscapePlayer;
class UUpgrade;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPlayerStatsComponent : public UStatsComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatsComponent();
	
	UFUNCTION()
	float GetGunDamage(const bool bHitWeakPoint) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UEffect>> EffectsToApplyWhenEnemyHitByPlayerGun;

	UFUNCTION()
	void OnEnemyHitByPlayerGun(AEnemyBase* Enemy, FHitResult HitResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AProjectEscapePlayer* Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UUpgrade*> OwningUpgrades;
	
	TMap<FGameplayTag, float*> StatMap
	{
		{PEGameplayTags::Stat_Gun_Damage, &GunDamage},
		{PEGameplayTags::Stat_Gun_DamageMultiplier, &GunDamageMultiplier},
		{PEGameplayTags::Stat_Gun_WeakPointMultiplier, &GunWeakPointMultiplier},
		{PEGameplayTags::Stat_Gun_FireRate, &FireRate},
		{PEGameplayTags::Stat_Gun_ReloadSpeedRate, &ReloadSpeedRate},
		{PEGameplayTags::Stat_Grab_DamageValue, &GrabDamageValue},
		{PEGameplayTags::Stat_Grab_ExplosionRadius, &GrabExplosionRadius},
	};

	UFUNCTION(BlueprintCallable)
	float GetStat(UPARAM(meta = (Categories = "Stat")) FGameplayTag GameplayTag);

	UFUNCTION(BlueprintCallable)
	void UpdateStat(UPARAM(meta = (Categories = "Stat")) FGameplayTag GameplayTag, const float NewValue);
	
	UFUNCTION(BlueprintCallable)
	void AddUpgrade(UUpgrade* Upgrade);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* UpgradeDataTable;

	UPROPERTY(EditAnywhere, Category= "Grab")
	float GrabDamageValue = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabExplosionRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Bullets")
	int MaxBullets = 13;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Bullets")
	int CurrentBullets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Bullets")
	float FireRate {1.1f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Bullets")
	float ReloadSpeedRate {1.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Damage")
	float GunDamage {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Damage")
	float GunDamageMultiplier {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun|Damage")
	float GunWeakPointMultiplier {2.f};


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
};