// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
		class UProgressBar* HPBar;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
		class UProgressBar* HPBarGuide;

	float TargetPercent = 1;

	void UpdateHP(int HP, int MaxHP);
};
