// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsFlying.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTService_IsFlying : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsFlying();

	void OnBecomeRelevant( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

};
