// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EnemySpawnableArea.h"

#include "Components/BoxComponent.h"


// Sets default values
AEnemySpawnableArea::AEnemySpawnableArea()
	:
	RootScene(CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"))),
	SpawnBoxExtent(CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Box Extent")))
{
	SetRootComponent(RootScene);
	SpawnBoxExtent->SetupAttachment(RootScene);
}

