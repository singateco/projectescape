﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "Components/TextBlock.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/RadialSlider.h"
#include "UI/ObjectiveWidget.h"


void UMainUI::SetCurrentBullets()
{
	BulletText->SetText(FText::FromString(
			FString::Printf(TEXT("%d | %d"), Player->PlayerStatsComponent->CurrentBullets, Player->PlayerStatsComponent->MaxBullets)
		));
	const float SliderValue = 1.f - (1.0f * Player->PlayerStatsComponent->CurrentBullets / Player->PlayerStatsComponent->MaxBullets);
	BulletSlider->SetValue(SliderValue);
}

void UMainUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Player=Cast<AProjectEscapePlayer>( GetWorld()->GetFirstPlayerController()->GetPawn() );
	EndQSkillUI();
	EndESkillUI();
}

void UMainUI::SetESkillCoolTimer(int32 CurrentTime, int32 MaxTime)
{

	PRB_ESkill->SetPercent( (float)CurrentTime/MaxTime);
	FString CurrentTimeStr =FString::FromInt( FMath::Max( CurrentTime, 0 ) );
	TXT_ESkill->SetText( FText::FromString( CurrentTimeStr ));
	
}
void UMainUI::SetQSkillCoolTimer( int32 CurrentTime, int32 MaxTime )
{

	PRB_QSkill->SetPercent( (float)CurrentTime / MaxTime );
	FString CurrentTimeStr=FString::FromInt( FMath::Max( CurrentTime, 0 ) );
	TXT_QSkill->SetText( FText::FromString( CurrentTimeStr ) );


}
void UMainUI::StartESkillUI()
{
	PRB_ESkill->SetVisibility( ESlateVisibility::Visible );
	TXT_ESkill->SetVisibility( ESlateVisibility::Visible );
	ChangeSkillColor(false, false);
}
void UMainUI::StartQSkillUI()
{
	PRB_QSkill->SetVisibility( ESlateVisibility::Visible );
	TXT_QSkill->SetVisibility( ESlateVisibility::Visible );
	ChangeSkillColor(true, false);
}
void UMainUI::EndESkillUI()
{
	PRB_ESkill->SetVisibility( ESlateVisibility::Hidden );
	TXT_ESkill->SetVisibility( ESlateVisibility::Hidden );
	PlayESkillCooldownAnim();
	ChangeSkillColor(false, true);
}
void UMainUI::EndQSkillUI()
{

	PRB_QSkill->SetVisibility( ESlateVisibility::Hidden );
	TXT_QSkill->SetVisibility( ESlateVisibility::Hidden );
	PlayQSkillCooldownAnim();
	ChangeSkillColor(true, true);
}

void UMainUI::SetCrossHairColor(const bool EnemySeen)
{
	if (EnemySeen)
	{
		CrossHair->SetBrushTintColor(FColor::Red);
	}
	else
	{
		CrossHair->SetBrushTintColor(FColor::White);
	}
}

void UMainUI::SetObjectiveVisibility(const bool Visible)
{
	if (Visible)
	{
		Objective->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Objective->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUI::PlayDamageAnimation()
{
	// 피격 시 애니메이션 재생
	PlayAnimation( DamageUIAnim );

}
