// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHP.h"

#include "Components/ProgressBar.h"
#include "Components/RadialSlider.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"

void UPlayerHP::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwnedPlayer)
	{
		OwnedPlayer->PlayerStatsComponent->OnHPChanged.AddUniqueDynamic(this, &UPlayerHP::UpdateHP);
	}
}

void UPlayerHP::UpdateHP(float MaxHP, float HP)
{
	HPProgress->SetPercent(HP / MaxHP);
}
