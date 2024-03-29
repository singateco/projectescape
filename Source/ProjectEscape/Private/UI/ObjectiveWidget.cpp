﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ObjectiveWidget.h"

#include "Components/CheckBox.h"
#include "Components/RichTextBlock.h"
#include "Enemy/EnemyBase.h"
#include "System/StageManager.h"

void UObjectiveWidget::SetupWave(const FWaveData& WaveData, TArray<AEnemyBase*>& InWaveEnemies)
{
	WaveEnemies = 0;
	EnemiesKilled = 0;

	for (auto tuple: WaveData.EnemyData)
	{
		WaveEnemies += tuple.Value;
	}

	for (auto Enemy: InWaveEnemies)
	{
		Enemy->OnEnemyDied.AddUniqueDynamic(this, &UObjectiveWidget::HandleEnemyDying);
	}
	
	GetWorld()->GetTimerManager().SetTimer(BonusUpdateTimer,
		FTimerDelegate::CreateUObject(this, &UObjectiveWidget::UpdateBonusText),
		0.1f,
		true);

	UpdateMainText();
}

void UObjectiveWidget::BonusFailed()
{
	GetWorld()->GetTimerManager().ClearTimer(BonusUpdateTimer);

	float BonusSeconds = 30.f;
	if (StageManager)
	{
		BonusSeconds = StageManager->BonusObjectiveSeconds;
	}
	FString CurrentBonusText = FString::Printf(TEXT("보너스 목표: %.f초 안에 클리어한다."), BonusSeconds);
	CurrentBonusText = TEXT("<Fail>") + CurrentBonusText + TEXT("</>");
	BonusTextBlock->SetText(FText::FromString(CurrentBonusText));
	BonusCheckBox->SetCheckedState(ECheckBoxState::Undetermined);
}

void UObjectiveWidget::StageFinished()
{
	bIsStageFinished = true;
	
	BonusCheckBox->RemoveFromParent();
	BonusTextBlock->RemoveFromParent();

	MainTextBlock->SetText(FText::FromString(TEXT("목표: 다음 층으로 이동한다.")));
	MainCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
}

void UObjectiveWidget::Init(AStageManager* _StageManager)
{
	StageManager = _StageManager;
	StageManager->OnWaveStarted.AddUniqueDynamic(this, &UObjectiveWidget::SetupWave);
	StageManager->OnBonusObjectiveTimeoutFailed.AddUniqueDynamic(this, &UObjectiveWidget::BonusFailed);
	StageManager->OnStageFinished.AddUniqueDynamic(this, &UObjectiveWidget::StageFinished);
}

void UObjectiveWidget::HandleEnemyDying(AEnemyBase* Enemy)
{
	EnemiesKilled++;
	UpdateMainText();
}

void UObjectiveWidget::UpdateBonusText()
{
	if (StageManager && GetWorld()->GetTimerManager().IsTimerActive(StageManager->BonusObjectiveHandle))
	{
		FString BonusText = FString::Printf(TEXT("보너스 목표: %.1f초 안에 클리어한다."), GetWorld()->GetTimerManager().GetTimerRemaining(StageManager->BonusObjectiveHandle));
		BonusTextBlock->SetText(FText::FromString(BonusText));
		BonusCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UObjectiveWidget::UpdateMainText()
{
	if (bIsStageFinished)
	{
		return;		
	}
	
	FString MainText = FString::Printf(TEXT("목표: 적을 섬멸한다. (%d/%d)"), EnemiesKilled, WaveEnemies);

	if (EnemiesKilled >= WaveEnemies)
	{
		MainText = TEXT("<Success>") + MainText + TEXT("</>");
		MainCheckBox->SetCheckedState(ECheckBoxState::Checked);

		if (GetWorld()->GetTimerManager().IsTimerActive(BonusUpdateTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(BonusUpdateTimer);
			FString CurrentBonusText = BonusTextBlock->GetText().ToString();
			CurrentBonusText = TEXT("<Success>") + CurrentBonusText + TEXT("</>");
			BonusTextBlock->SetText(FText::FromString(CurrentBonusText));
			BonusCheckBox->SetCheckedState(ECheckBoxState::Checked);
		}
	}
	else
	{
		MainCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
	
	MainTextBlock->SetText(FText::FromString(MainText));
}
