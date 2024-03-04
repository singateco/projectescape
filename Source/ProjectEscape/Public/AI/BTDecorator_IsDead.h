// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsDead.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTDecorator_IsDead : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTDecorator_IsDead();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
