// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Dash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBTTask_Dash : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

	UBTTask_Dash();

	void PlayAnimMontage();

	virtual EBTNodeResult::Type ExecuteTask( UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory ) override;

	UPROPERTY( EditAnywhere, BlueprintReadWrite)
	float DashForce = 1000.f ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	FVector MoveVector;

	UPROPERTY( EditDefaultsOnly )
	UAnimMontage* LeftDashMontage;

	UPROPERTY( EditDefaultsOnly )
	UAnimMontage* RightDashMontage;

	UPROPERTY( EditDefaultsOnly )
	UAnimMontage* BackDashMontage;

};
