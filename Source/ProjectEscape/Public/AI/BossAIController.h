// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ABossAIController : public AAIController
{
	GENERATED_BODY()


public:
	explicit ABossAIController(FObjectInitializer const& ObjectInitializer);

	virtual void OnPossess( APawn* InPawn ) override;
	
};
