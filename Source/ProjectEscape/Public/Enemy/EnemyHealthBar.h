﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	UFUNCTION(BlueprintCallable)
	void UpdateHP(float MaxHP, float HP);

	UFUNCTION()
	void HPtoZero();

	/*UFUNCTION(BlueprintCallable)
	void SetName();*/

protected:
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UProgressBar* HPBarGuide;

	/*UPROPERTY( EditInstanceOnly, meta = (BindWidget) )
	class UTextBlock* EnemyName;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBase* OwnedEnemy;

	float TargetPercent = 1;
};
