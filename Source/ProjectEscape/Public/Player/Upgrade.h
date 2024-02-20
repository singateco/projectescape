// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/Effect.h"
#include "UObject/Object.h"
#include "Upgrade.generated.h"

USTRUCT()
struct FUpgradeData: public FTableRowBase
{
	GENERATED_BODY()

	// 업그레이드의 이름.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name {EName::None};

	// UI에서 표시할 설명.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine))
	FText Description;

	// UI에서 표시할 이미지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Image;

	// 플레이어에게 부여할 이펙트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UEffect>> PlayerEffects;

	// 총 쏠때 적에게 부여할 이펙트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UEffect>> GunEffects;

	// 던질때 적에게 부여할 이펙트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UEffect>> GrabEffects;

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

	UPROPERTY(BlueprintReadWrite)
	int32 UpgradeUID;
	
	UPROPERTY(BlueprintReadWrite)
	FUpgradeData StaticData;
};
