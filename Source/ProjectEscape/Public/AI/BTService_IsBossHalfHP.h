// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsBossHalfHP.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTService_IsBossHalfHP : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTService_IsBossHalfHP();

	void OnBecomeRelevant( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;


};
