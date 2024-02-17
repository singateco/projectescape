// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStaminaUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPlayerStaminaUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class URadialSlider* StaminaSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	URadialSlider* StaminaSlider_Guide;

	UFUNCTION()
	void UpdateStamina(const float MaxStamina, const float CurrentStamina);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGuideAnim();

	UPROPERTY(BlueprintReadWrite)
	bool bPlayingGuideAnim {false};
	
	UPROPERTY()
	float StaminaValue {1.f};

	UPROPERTY()
	float AnimEnableDelta {0.1f};
};