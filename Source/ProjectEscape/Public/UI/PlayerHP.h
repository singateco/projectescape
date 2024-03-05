// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHP.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPlayerHP : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* HPText;
		
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* HPProgress;
	
	UFUNCTION()
	void UpdateHP(float MaxHP, float HP);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHPGuideAnim();
	
	UPROPERTY(BlueprintReadWrite)
	float HPPercent;
	
	UPROPERTY(VisibleAnywhere)
	class AProjectEscapePlayer* OwnedPlayer;
};
