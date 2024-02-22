// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "BombEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ABombEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABombEnemy( const FObjectInitializer& ObjectInitializer );

	virtual void BeginPlay() override;

};
