// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Upgrade.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UpgradeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UUpgradeSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	UDataTable* UpgradeDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUpgradeData> StaticUpgradeDatas;

	UFUNCTION(BlueprintCallable)
	FUpgradeData GetRandomUpgradeData() const;
	
	UFUNCTION(BlueprintCallable)
	UUpgrade* MakeUpgradeFromStaticData(const FUpgradeData& Data);

	UFUNCTION(BlueprintCallable)
	UUpgrade* GetRandomUpgrade();
	
	int32 UpgradeCounter {0};
};
