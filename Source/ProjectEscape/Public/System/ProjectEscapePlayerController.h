// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectEscapePlayerController.generated.h"

/**
 * 
 */

class UPlayerStaminaUI;
class UPlayerHP;
class UMainUI;

UCLASS()
class PROJECTESCAPE_API AProjectEscapePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHP> PlayerHPWidgetClass;

	UPROPERTY()
	UPlayerHP* PlayerHPWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerStaminaUI> PlayerStaminaUIWidgetClass;

	UPROPERTY()
	UPlayerStaminaUI* PlayerStaminaUIWidget;
		
private:
	UPROPERTY( EditDefaultsOnly, Meta=(AllowPrivateAccess) )
	TSubclassOf<UMainUI> InGameWIdgetClass;

	UPROPERTY( EditDefaultsOnly, Meta=(AllowPrivateAccess) )
	UMainUI* InGameWIdget;
};
