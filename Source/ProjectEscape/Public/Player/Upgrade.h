// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Upgrade.generated.h"

USTRUCT()
struct FUpgradeData: public FTableRowBase
{
	GENERATED_BODY()

	// 업그레이드의 이름.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	// UI에서 표시할 설명.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> StatsToAlwaysModify;

	// 업그레이드의 희귀도.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Rarity"))
	FGameplayTag Rarity;
};

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FUpgradeData StaticData;
};
