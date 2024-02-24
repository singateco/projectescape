// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "SniperEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ASniperEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ASniperEnemy(const FObjectInitializer& ObjectInitializer);

	UPROPERTY( EditDefaultsOnly )
	UStaticMeshComponent* LaserBeam;
};
