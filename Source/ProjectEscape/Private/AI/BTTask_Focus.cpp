// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Focus.h"

#include "AI/BossAIController.h"
#include "Player/ProjectEscapePlayer.h"

UBTTask_Focus::UBTTask_Focus()
{
	NodeName = TEXT( "Focus" );

}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* Cont = Cast<ABossAIController>( OwnerComp.GetAIOwner() );

	if( Cont )
	{
		auto Player = Cast<AProjectEscapePlayer>( GetWorld()->GetFirstPlayerController()->GetPawn() );
		Cont->SetFocus( Player );

	}
	FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
	return EBTNodeResult::Succeeded;
}
