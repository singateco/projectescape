// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseFSM.h"
#include "SniperEnemyFSM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API USniperEnemyFSM : public UEnemyBaseFSM
{
	GENERATED_BODY()

public:
	USniperEnemyFSM();

	UPROPERTY()
	FVector TargetPostion;
	
	UPROPERTY( EditAnyWhere, BlueprintReadWrite, Category="Rifle" )
	TSubclassOf<class AEnemyBullet> EnemyBulletFactory;

	void TickAttack() override;

	void OnTakeDamage( float Damage_Unused ) override;

	void TickDie() override;
};
