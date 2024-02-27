﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ABossEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABossEnemy( const FObjectInitializer& ObjectInitializer );

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* Tree;

	UBehaviorTree* GetBehaviorTree();


};
