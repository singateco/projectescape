// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SpawnThrowActor.h"
#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "Enemy/EnemyThrowActor.h"


UBTTask_SpawnThrowActor::UBTTask_SpawnThrowActor()
{
	NodeName = TEXT( "SpawnThrowActor" );

    static ConstructorHelpers::FClassFinder<AEnemyThrowActor> ThrowActorFinder{ TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/BP_PickableActor.BP_PickableActor_C'" ) };

    if ( ThrowActorFinder.Succeeded() )
    {
        ThrowActorFactory = ThrowActorFinder.Class;
    }

}

EBTNodeResult::Type UBTTask_SpawnThrowActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABossAIController* Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
    APawn* Pawn=Cont->GetPawn();
    ABossEnemy* Boss = Cast<ABossEnemy>( Pawn );

    if(Boss )
    {
        if(ThrowActor ) // 여기가 없음
        {
            ThrowActor=GetWorld()->SpawnActor<AEnemyThrowActor>( ThrowActorFactory, Boss->ThrowRef->GetComponentTransform() );

            ThrowActor->AttachToComponent( Boss->ThrowRef, FAttachmentTransformRules::SnapToTargetNotIncludingScale );

            FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
            return EBTNodeResult::Succeeded;
        }
    	
        
    }
    return EBTNodeResult::Failed;


}
