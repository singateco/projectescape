// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetUIComp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UTargetUIComp : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidgetAnim), Transient )
	class UWidgetAnimation* TargetDectionAnim;


	void PlayDamageAnimation();
};
