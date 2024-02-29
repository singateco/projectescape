// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"

#include "AI/BossAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(auto* const cont = Cast<ABossAIController>(OwnerComp.GetAIOwner()) )
	{
		if(auto const npc = cont->GetPawn())
		{
			auto const Origin=npc->GetActorLocation();

			if(auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()) )
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc) )
				{
					Loc.Location = Origin + (Loc.Location - Origin).GetSafeNormal() * SearchRadius;
					OwnerComp.GetBlackboardComponent()->SetValueAsVector( GetSelectedBlackboardKey(), Loc.Location );
				}

				FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
				return EBTNodeResult::Succeeded;
			}
			
		}
	}
	return EBTNodeResult::Failed;

}
