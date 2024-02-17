// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStaminaUI.h"

#include "Components/RadialSlider.h"

void UPlayerStaminaUI::UpdateStamina(const float MaxStamina, const float CurrentStamina)
{
	StaminaValue = 1.f - (CurrentStamina / MaxStamina);
	StaminaSlider->SetValue(StaminaValue);
}

void UPlayerStaminaUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (StaminaSlider_Guide->Value != StaminaValue)
	{
		constexpr float InterpSpeed = 1.5;
		StaminaSlider_Guide->SetValue(FMath::FInterpTo(StaminaSlider_Guide->Value, StaminaValue, InDeltaTime, InterpSpeed));
	}
}
