// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_IsFlying.h"

#include "AI/BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_IsFlying::UBTService_IsFlying()
{
	bNotifyBecomeRelevant=true;
	NodeName=TEXT( "Is Flying" );
}

void UBTService_IsFlying::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	auto Boss=Cast<ABossEnemy>( Cont->GetPawn() );

	if ( Boss )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool( GetSelectedBlackboardKey(), Boss->GetCharacterMovement()->IsFlying());
	}
}
