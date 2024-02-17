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

	UPROPERTY()
	float StaminaValue {1.f};
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
