// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BossEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/ProjectEscapePlayer.h"


ABossAIController::ABossAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
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

void ABossAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "SightConfig" ) );
	if(SightConfig )
	{
		SetPerceptionComponent( *CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "Perception Component" ) ) );
		SightConfig->SightRadius = 3000.0f; // 시야 반경 설정
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees=90.0f; // 시야 각도 설정
		SightConfig->DetectionByAffiliation.bDetectEnemies=true; // 적 감지 설정
		SightConfig->DetectionByAffiliation.bDetectFriendlies=true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals=true; // 적 감지 설정

		GetPerceptionComponent()->SetDominantSense( *SightConfig->GetSenseImplementation() );
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic( this, &ABossAIController::OnTargetDetected );
		GetPerceptionComponent()->ConfigureSense( *SightConfig );
	}
	

}

void ABossAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{

	auto SeeActor=Cast<AProjectEscapePlayer>( Actor );
	if ( SeeActor )
	{
		GetBlackboardComponent()->SetValueAsBool( "CanSeePlayer", Stimulus.WasSuccessfullySensed() );
	}
}




