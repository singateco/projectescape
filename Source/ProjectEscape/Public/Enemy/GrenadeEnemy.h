// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "GrenadeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AGrenadeEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AGrenadeEnemy( const FObjectInitializer& ObjectInitializer );

};
