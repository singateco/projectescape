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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBonusObjectiveTimeoutFailed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaveFinished, FWaveData, FinishedWaveData, FWaveData, NextWaveData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaveStarted, const FWaveData&, WaveData, TArray<AEnemyBase*>&, WaveEnemies);

UCLASS()
class PROJECTESCAPE_API AStageManager : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void FirstWaveSpawn();
	
	UPROPERTY()
	USceneComponent* RootSceneComp;

	UPROPERTY(EditAnywhere, meta = (RowType = "StageData"))
	FDataTableRowHandle StageDataTableRow;

	UPROPERTY(VisibleAnywhere)
	FStageData StageData;

	UPROPERTY(BlueprintAssignable)
	FBonusObjectiveTimeoutFailed OnBonusObjectiveTimeoutFailed;
	
	UPROPERTY(BlueprintAssignable)
	FStageFinished OnStageFinished;

	UPROPERTY(BlueprintAssignable)
	FWaveFinished OnWaveFinished;

	UPROPERTY(BlueprintAssignable)
	FWaveStarted OnWaveStarted;

	UPROPERTY(EditAnywhere)
	TArray<AEnemyBase*> ThisWaveEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bBonusReward {false};

	UPROPERTY(VisibleAnywhere)
	bool bIsFinalWave {false};

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWaveIndex {0};

	UPROPERTY(EditAnywhere)
	float BetweenWavesDelaySeconds {7.0f};

	UPROPERTY(EditAnywhere)
	float BonusObjectiveSeconds {30.f};

	UPROPERTY(EditAnywhere)
	FTimerHandle BonusObjectiveHandle;

	UFUNCTION()
	void HandleEnemyDestroyed(AEnemyBase* DestroyedActor);

	UFUNCTION()
	void BonusObjectiveTimeoutFailed();
	
	UFUNCTION()
	void SpawnWave(const FWaveData& WaveData);
	
	// Sets default values for this actor's properties
	AStageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
