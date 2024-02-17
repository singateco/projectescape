// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStaminaUI.h"

#include "Components/RadialSlider.h"

void UPlayerStaminaUI::UpdateStamina(const float MaxStamina, const float CurrentStamina)
{
	const float NewStaminaValue = 1.f - (CurrentStamina / MaxStamina);
	const float StaminaDelta = abs(StaminaValue - NewStaminaValue);
	StaminaValue = NewStaminaValue;
	StaminaSlider->SetValue(StaminaValue);

	if (StaminaDelta >= AnimEnableDelta)
	{
		PlayGuideAnim();
	}
	else
	{
		if (!bPlayingGuideAnim) StaminaSlider_Guide->SetValue(StaminaValue);
	}
}
