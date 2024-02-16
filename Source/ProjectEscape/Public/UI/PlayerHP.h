// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHP.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPlayerHP : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void UpdateHP(float MaxHP, float HP);
	
	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	UProgressBar* HPBarGuide;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UTextBlock* HPText;
	
	UPROPERTY(VisibleAnywhere)
	class AProjectEscapePlayer* OwnedPlayer;

	float TargetPercent = 1;
};
