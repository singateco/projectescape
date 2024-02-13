// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RifleEnemy.h"

#include "Enemy/RifleEnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

ARifleEnemy::ARifleEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/SWATGuy.SWATGuy'"));

	if (TempEnemyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempEnemyMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	this->HP = 3;

}

