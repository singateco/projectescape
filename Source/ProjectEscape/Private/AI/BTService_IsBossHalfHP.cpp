// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_IsBossHalfHP.h"

#include "AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BossEnemy.h"

UBTService_IsBossHalfHP::UBTService_IsBossHalfHP()
{
	bNotifyBecomeRelevant = true;
	NodeName=TEXT( "Is Boss Half HP" );
}

void UBTService_IsBossHalfHP::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	auto Boss=Cast<ABossEnemy>( Cont->GetPawn() );

	if(Boss )
	{
		float BossMaxHP = Boss->GetStatsComponent()->GetMaxHP();
		float BossHP = Boss->GetStatsComponent()->GetHP();
		
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( GetSelectedBlackboardKey(), BossHP <= BossMaxHP * 0.6 );
	}

}
