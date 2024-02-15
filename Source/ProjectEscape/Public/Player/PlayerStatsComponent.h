// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/StatsComponent.h"
#include "PlayerStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPlayerStatsComponent : public UStatsComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatsComponent();

	UPROPERTY( EditDefaultsOnly, Category="Damage" )
	float GrabDamageValue=500.0f;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
