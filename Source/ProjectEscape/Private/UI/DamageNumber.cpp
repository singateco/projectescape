// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageNumber.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"

void UDamageNumber::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	FNumberFormattingOptions Options;
	Options.AlwaysSign = false;
	Options.RoundingMode = HalfFromZero;
	Options.MinimumFractionalDigits = 0;
	Options.MaximumFractionalDigits = 1;
	
	TextBlock->SetText(FText::AsNumber(DamageToDisplay, &Options));
	Cast<UCanvasPanelSlot>(TextBlock->Slot)->SetPosition(FVector2D(FMath::FRandRange(-20.f, 20.f), FMath::FRandRange(-10.f, 10.f)));
	
}

void UDamageNumber::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	FVector2D PositionToDisplay;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && Actor)
	{
		PC->ProjectWorldLocationToScreen(Actor->GetActorLocation(), PositionToDisplay);
		SetPositionInViewport(PositionToDisplay);
	}
}
