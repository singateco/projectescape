// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AEnemyAIController();

	virtual void BeginPlay() override;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category="AI Perception" )
		class UEnemyAIPerception* EnemyAIPerception;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category="AI Perception" )
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		class UEnemyBaseFSM* FSM;

	UFUNCTION()
		void OnSeePlayer( AActor* Actor, FAIStimulus Stimulus );

	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;

};
