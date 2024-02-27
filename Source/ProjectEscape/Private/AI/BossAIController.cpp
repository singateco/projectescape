// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/BossEnemy.h"


ABossAIController::ABossAIController(FObjectInitializer const& ObjectInitializer)
{

}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(ABossEnemy* BossEnemy = Cast<ABossEnemy>(InPawn) )
	{
		if ( UBehaviorTree* const Tree = BossEnemy->GetBehaviorTree() )
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard( Tree->BlackboardAsset, BlackboardComponent );
			Blackboard = BlackboardComponent;
			RunBehaviorTree( Tree );
		}
	}

}




