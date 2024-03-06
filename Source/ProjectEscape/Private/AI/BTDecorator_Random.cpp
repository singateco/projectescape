// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_Random.h"

UBTDecorator_Random::UBTDecorator_Random()
{
	NodeName=TEXT( "RandomChance" );
}

bool UBTDecorator_Random::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return FMath::RandRange( 0.f, 1.f ) < Chance;
}
