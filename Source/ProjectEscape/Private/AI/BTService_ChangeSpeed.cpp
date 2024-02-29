// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_ChangeSpeed.h"

#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant=true;
	NodeName=TEXT( "Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	auto Boss=Cast<ABossEnemy>( Cont->GetPawn() );

	if(Boss )
	{
		Boss->GetCharacterMovement()->MaxWalkSpeed = Speed;

	}
}
