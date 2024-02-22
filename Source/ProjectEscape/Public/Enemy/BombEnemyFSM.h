// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseFSM.h"
#include "BombEnemyFSM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UBombEnemyFSM : public UEnemyBaseFSM
{
	GENERATED_BODY()


public:
	UBombEnemyFSM();

	UPROPERTY( EditAnywhere, Category=Effect )
		class UNiagaraSystem* ExplosionEffect;

	UPROPERTY( EditAnywhere, Category=Effect )
		USoundBase* ExplosionSound;

	UPROPERTY( EditAnywhere )
		float SphereRadius = 300.0f;

	void TickMove() override;

	void TickAttack() override;

	void TickDie() override;

	UFUNCTION( BlueprintCallable )
	void Explosion();
};
