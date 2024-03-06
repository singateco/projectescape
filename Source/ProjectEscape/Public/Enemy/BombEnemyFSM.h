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

	UPROPERTY( EditAnywhere )
		float ShakeRadius=2000.0f;

	UPROPERTY( EditAnywhere, Category=Effect )
	TSubclassOf<class UCameraShakeBase> CameraShake;

	void TickMove() override;

	void TickAttack() override;

	void TickDie() override;

	void OnTakeDamage(float Damage_Unused) override;

	UFUNCTION( BlueprintCallable )
	void Explosion();
};
