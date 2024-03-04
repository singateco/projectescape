// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/StageManager.h"
#include "ObjectiveWidget.generated.h"

class UCheckBox;
class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void BonusFailed();

	UFUNCTION()
	void Init(AStageManager* _StageManager);

	UPROPERTY()
	AStageManager* StageManager;
	
protected:

	UPROPERTY()
	FTimerHandle BonusUpdateTimer;
	
	UFUNCTION()
	void HandleEnemyDying(AEnemyBase* Enemy);

	void UpdateBonusText();

	UFUNCTION()
	void SetupWave(const FWaveData& WaveData, TArray<AEnemyBase*>& InWaveEnemies);

	UPROPERTY(VisibleAnywhere)
	int32 WaveEnemies;

	UPROPERTY(VisibleAnywhere)
	int32 EnemiesKilled;

	UFUNCTION()
	void UpdateMainText();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	URichTextBlock* MainTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	URichTextBlock* BonusTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	UCheckBox* MainCheckBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	UCheckBox* BonusCheckBox;
};
