// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnableArea.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTESCAPE_API AEnemySpawnableArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawnableArea();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* SpawnBoxExtent;

};
