// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyAIController::HomePosKey( TEXT( "HomePos" ) );
const FName AEnemyAIController::PatrolPosKey( TEXT( "PatrolPos" ) );

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject( TEXT( "/Script/AIModule.BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'" ) );

	if(BBObject.Succeeded())
	{
		BBAsset= BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject( TEXT( "/Script/AIModule.BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'" ) );

	if(BTObject.Succeeded())
	{
		BTAsset= BTObject.Object;
	}

	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess( InPawn );
	UBlackboardComponent* BlackboardComponent= Blackboard;

	if(UseBlackboard(BBAsset, BlackboardComponent))
	{
		Blackboard->SetValueAsVector( HomePosKey, InPawn->GetActorLocation());
	}
}
