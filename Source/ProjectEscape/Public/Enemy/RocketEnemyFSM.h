// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseFSM.h"
#include "RocketEnemyFSM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API URocketEnemyFSM : public UEnemyBaseFSM
{
	GENERATED_BODY()

public:
	URocketEnemyFSM();

	UPROPERTY( EditAnywhere, Category = "Rocket" )
	TSubclassOf<class ARocket> EnemyRocketFactory;

	UPROPERTY( EditDefaultsOnly )
	ARocket* EnemyRocket;

	UPROPERTY( EditAnywhere )
	float RocketSpeed = 4000.0f;

	void TickAttack() override;

	void FireRocket();

};
