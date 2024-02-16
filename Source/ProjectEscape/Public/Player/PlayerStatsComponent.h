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

	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float GrabDamageValue=500.0f;

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	int MaxBullets = 30;

	UPROPERTY( EditDefaultsOnly, Category="Fire" )
	int CurrentBullets;

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
	float GunWeakPointMultiplier {1.5f};
	
	float InitialGunDamage;
	float InitialGunDamageMultiplier;
};