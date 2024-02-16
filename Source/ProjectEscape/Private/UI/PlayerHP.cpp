// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHP.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"

void UPlayerHP::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwnedPlayer)
	{
		UpdateHP(OwnedPlayer->PlayerStatsComponent->GetMaxHP(), OwnedPlayer->PlayerStatsComponent->GetHP());
		OwnedPlayer->PlayerStatsComponent->OnHPChanged.AddUniqueDynamic(this, &UPlayerHP::UpdateHP);
	}
}

void UPlayerHP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float newPercent = FMath::Lerp(HPBarGuide->GetPercent(), TargetPercent, InDeltaTime * 5);
	HPBarGuide->SetPercent(newPercent);
}

void UPlayerHP::UpdateHP(float MaxHP, float HP)
{
	FText HPString = FText::FromString(FString::Printf(TEXT("%.f/%.f"), HP, MaxHP));
	HPText->SetText(HPString);

	TargetPercent = HP / MaxHP;
	HPBar->SetPercent(TargetPercent);
}
