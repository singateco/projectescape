// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "Components/TextBlock.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"



void UMainUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	Player=Cast<AProjectEscapePlayer>( GetWorld()->GetFirstPlayerController()->GetPawn() );
	
}

void UMainUI::SetCurrentBullets()
{
	TXT_CurrentBullets->SetText( FText::AsNumber( Player->PlayerStatsComponent->CurrentBullets ) );
}
