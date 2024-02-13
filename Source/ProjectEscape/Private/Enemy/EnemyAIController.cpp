// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

#include "Enemy/EnemyAIPerception.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBaseFSM.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/ProjectEscapePlayer.h"

AEnemyAIController::AEnemyAIController()
{
	EnemyAIPerception=CreateDefaultSubobject<UEnemyAIPerception>( TEXT( "EnemyAIPerception" ) );

	SightConfig=CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "SightConfig" ) );
	SightConfig->SightRadius=3000.0f; // 시야 반경 설정
	SightConfig->LoseSightRadius=3200.0f; // 시야를 잃는 반경 설정
	SightConfig->PeripheralVisionAngleDegrees=90.0f; // 시야 각도 설정
	SightConfig->DetectionByAffiliation.bDetectEnemies=true; // 적 감지 설정
	SightConfig->DetectionByAffiliation.bDetectFriendlies=true; // 적 감지 설정
	SightConfig->DetectionByAffiliation.bDetectNeutrals=true; // 적 감지 설정

	// SightConfig를 EnemyAIPerception에 추가
	EnemyAIPerception->ConfigureSense( *SightConfig );
	SetPerceptionComponent( *EnemyAIPerception );

	

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyAIController::OnSeePlayer(AActor* Actor, FAIStimulus Stimulus)
{
    auto SeeActor = Cast<AProjectEscapePlayer>(Actor);
    if (SeeActor)
    {
        FSM->bCanSeePlayer = Stimulus.WasSuccessfullySensed();
    }
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyBase* EnemyPawn = Cast<AEnemyBase>( InPawn );
	check( EnemyPawn );
	FSM = EnemyPawn->EnemyBaseFSM;

	if ( EnemyAIPerception )
	{
		EnemyAIPerception->OnTargetPerceptionUpdated.AddDynamic( this, &AEnemyAIController::OnSeePlayer );
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


