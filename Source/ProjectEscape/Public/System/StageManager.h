// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageManager.generated.h"

class AEnemyBase;



USTRUCT(BlueprintType, Blueprintable)
struct FWaveData: public FTableRowBase
{
	GENERATED_BODY()

	// 소환될 적의 종류와 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TSubclassOf<AEnemyBase>, int> EnemyData;

	// 마지막 웨이브인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFinalWave {false};
};

USTRUCT(BlueprintType)
struct FStageData: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWaveData> Waves;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaveFinished, FWaveData, FinishedWaveData, FWaveData, NextWaveData);

UCLASS()
class PROJECTESCAPE_API AStageManager : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
	USceneComponent* RootSceneComp;

	UPROPERTY(EditAnywhere, meta = (RowType = "StageData"))
	FDataTableRowHandle StageDataTableRow;

	UPROPERTY(VisibleAnywhere)
	FStageData StageData;

	UPROPERTY(BlueprintAssignable)
	FStageFinished OnStageFinished;

	UPROPERTY(BlueprintAssignable)
	FWaveFinished OnWaveFinished;

	UPROPERTY(EditAnywhere)
	TArray<AEnemyBase*> ThisWaveEnemy;

	UPROPERTY(VisibleAnywhere)
	bool bIsFinalWave {false};

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWaveIndex {0};

	UFUNCTION()
	void HandleEnemyDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void SpawnWave(const FWaveData& WaveData);
	
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
