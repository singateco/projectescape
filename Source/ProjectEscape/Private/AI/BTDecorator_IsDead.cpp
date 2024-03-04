// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsDead.h"

#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"

UBTDecorator_IsDead::UBTDecorator_IsDead()
{
	NodeName = TEXT( "IsDead" );
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	auto Boss = Cast<ABossEnemy>( Cont->GetPawn() );

	bool bIsDead = false;

	if(Boss )
	{
		bIsDead = Boss->GetIsDead();
	}

	return bIsDead;
}
