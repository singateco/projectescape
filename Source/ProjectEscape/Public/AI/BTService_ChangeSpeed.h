// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTService_ChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_ChangeSpeed();
	void OnBecomeRelevant( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	UPROPERTY( EditAnywhere )
	float Speed = 200.f;


};
