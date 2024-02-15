// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "RifleEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ARifleEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ARifleEnemy();

	virtual void BeginPlay() override;

	// --------------------- Variable ---------------------------
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class URifleEnemyFSM* RifleEnemyFSM;*/
	
	// --------------------- Function ---------------------------

	

};
