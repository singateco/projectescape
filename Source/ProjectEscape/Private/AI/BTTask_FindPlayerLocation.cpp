// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if ( auto Player = UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 ) )
	{
		FVector PlayerLocation = Player->GetActorLocation();
		if(SearchRandom )
		{
			FNavLocation Loc;
			if(auto NavSys = UNavigationSystemV1::GetCurrent(GetWorld()) )
			{
				if(NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc) )
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector( GetSelectedBlackboardKey(), Loc.Location );
					FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
					return EBTNodeResult::Succeeded;
				}
			}
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector( GetSelectedBlackboardKey(), PlayerLocation );
			FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
