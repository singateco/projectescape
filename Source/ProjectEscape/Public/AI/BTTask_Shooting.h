﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Shooting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTTask_Shooting : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_Shooting();

	UPROPERTY( EditAnywhere )
	TSubclassOf<class AEnemyBullet> EnemyBulletFactory;

	UPROPERTY( EditAnywhere )
	USoundBase* ShootingSound;

	UPROPERTY( EditAnywhere )
	UParticleSystem* MuzzleFlash;

	UPROPERTY( EditAnywhere )
	UAnimMontage* ShootingMontage;

	UPROPERTY( EditAnywhere )
	float Accuracy = 5.0f;

	UPROPERTY( EditAnywhere )
	float Spread = 3.0f;

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;
};
