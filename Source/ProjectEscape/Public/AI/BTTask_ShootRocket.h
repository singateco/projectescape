// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ShootRocket.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTTask_ShootRocket : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ShootRocket();

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<class ARocket> RocketFactory;

	UPROPERTY( EditDefaultsOnly )
	USoundBase* ShootingSound;

	UPROPERTY( EditAnywhere )
	UParticleSystem* MuzzleFlash;

	UPROPERTY( EditDefaultsOnly )
	UAnimMontage* ShootingMontage;

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

};
