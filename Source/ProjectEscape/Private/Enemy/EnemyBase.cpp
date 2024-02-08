// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Enemy/EnemyAIPerception.h"
#include "Enemy/EnemyBaseFSM.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyBaseFSM = CreateDefaultSubobject<UEnemyBaseFSM>(TEXT("EnemyBaseFSM"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

}


