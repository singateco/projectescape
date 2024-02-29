// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTTask_FindPatrolPos : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindPatrolPos( FObjectInitializer const& ObjectInitializer );

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

private:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true") )
	float SearchRadius = 500;


};
