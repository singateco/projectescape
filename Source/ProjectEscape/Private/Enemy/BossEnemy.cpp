// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy.h"

ABossEnemy::ABossEnemy(const FObjectInitializer& ObjectInitializer)
	:
	Super( ObjectInitializer )

{
	PrimaryActorTick.bCanEverTick=true;

}
