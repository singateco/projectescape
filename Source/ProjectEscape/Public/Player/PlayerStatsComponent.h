// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/StatsComponent.h"
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

	UFUNCTION()
	void CalculateStat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* UpgradeDataTable;

	UPROPERTY(EditAnywhere, Category="Grab")
	float GrabDamageValue = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Grab")
	float GrabExplosionRadius = 300.f;

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	int MaxBullets = 13;

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	int CurrentBullets;

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	float FireRate {1.25f};

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	float ReloadSpeedRate {1.f};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeStat();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess))
	float GunDamage {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess))
	float GunDamageMultiplier {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess))
	float GunWeakPointMultiplier {2.f};
	
	float InitialGunDamage;
	float InitialGunDamageMultiplier;
};