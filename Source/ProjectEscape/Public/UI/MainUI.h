// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */

class UTextBlock;
class AProjectEscapePlayer;

UCLASS()
class UMainUI : public UUserWidget
{
	GENERATED_BODY()
public:

public:
	UPROPERTY(EditDefaultsOnly)
	AProjectEscapePlayer* Player;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UTextBlock* TXT_CurrentBullets;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UTextBlock* TXT_MaxBullets;

	UFUNCTION()
	void SetCurrentBullets();

protected:
	virtual void NativeOnInitialized() override;
};
