// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseFSM.h"
#include "GrenadeEnemyFSM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UGrenadeEnemyFSM : public UEnemyBaseFSM
{
	GENERATED_BODY()

public:
	UGrenadeEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY( EditAnyWhere, BlueprintReadWrite, Category="Rifle" )
		TSubclassOf<class AEnemyBullet> EnemyBulletFactory;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Rifle" )
		AEnemyBullet* EnemyBullet;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="EnemyState" )
		float GrenadeTime = 5.0f;

	void TickAttack() override;

};
