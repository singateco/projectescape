﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyBaseFSM.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
		class AEnemyBase* Enemy;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		EEnemyState State;

	// 앞뒤로 움직이는 속력
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		float Speed;

	// 좌우로 움직이는 속력
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		float Direction;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		bool IsFalling;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="AnimMontage" )
		UAnimMontage* EnemyHitMontage;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="AnimMontage" )
		UAnimMontage* EnemyShootMontage;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category="AnimMontage" )
		UAnimMontage* EnemyDieMontage;

	void PlayShootMontage();
	void PlayHitAnimMontage();
	void PlayDieAnimMontage();


	void AnimNotify_DamageEnd();
};
