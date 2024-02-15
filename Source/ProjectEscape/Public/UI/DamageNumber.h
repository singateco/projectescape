// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumber.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UDamageNumber : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TextBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageToDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;
};
