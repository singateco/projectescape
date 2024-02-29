// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SpawnThrowActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTTask_SpawnThrowActor : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_SpawnThrowActor();

	UPROPERTY( EditDefaultsOnly )
	TSubclassOf<class AEnemyThrowActor> ThrowActorFactory;

	UPROPERTY( EditDefaultsOnly )
	AEnemyThrowActor* ThrowActor;

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;
};
