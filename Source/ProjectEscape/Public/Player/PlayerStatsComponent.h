// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/StatsComponent.h"
#include "PlayerStatsComponent.generated.h"


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
	float GetGunDamage() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AProjectEscapePlayer* Player;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UUpgrade*> OwningUpgrades;

	UFUNCTION()
	void CalculateStat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* UpgradeDataTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeStat();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess))
	float GunDamage {1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess))
	float GunDamageMultiplier {1};
	
	float InitialGunDamage;
	float InitialGunDamageMultiplier;
};