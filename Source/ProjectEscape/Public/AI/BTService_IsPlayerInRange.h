// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTService_IsPlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsPlayerInRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float AttackRange = 3000.f;

};
