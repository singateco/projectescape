// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_IsPlayerInRange.h"

#include "AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BossEnemy.h"
#include "Kismet/GameplayStatics.h"


UBTService_IsPlayerInRange::UBTService_IsPlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT( "Is Player In Range" );
}

void UBTService_IsPlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Cont = Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	auto Boss = Cast<ABossEnemy>( Cont->GetPawn() );

	auto Player = UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 );

	OwnerComp.GetBlackboardComponent()->SetValueAsBool( GetSelectedBlackboardKey(), Boss->GetDistanceTo( Player ) <= AttackRange );
}
