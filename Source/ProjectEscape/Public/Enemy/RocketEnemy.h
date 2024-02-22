// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "RocketEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ARocketEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ARocketEnemy( const FObjectInitializer& ObjectInitializer );
};
