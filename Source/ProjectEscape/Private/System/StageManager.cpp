// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageManager.h"
#include "Enemy/EnemyBase.h"

void AStageManager::HandleEnemyDestroyed(AActor* DestroyedActor)
{
	AEnemyBase* CastedActor = Cast<AEnemyBase>(DestroyedActor);
	ThisWaveEnemy.RemoveSwap(CastedActor);
	
	if (ThisWaveEnemy.IsEmpty())
	{
		if (bIsFinalWave)
		{
			// Do something
			UE_LOG(LogTemp, Warning, TEXT("Stage Won!"))
			return;
		}

		CurrentWaveIndex++;
		if (StageData.Waves.Num() > CurrentWaveIndex)
		{
			SpawnWave(StageData.Waves[CurrentWaveIndex]);
		}
	}
}

void AStageManager::SpawnWave(const FWaveData& WaveData)
{
	TMap<TSubclassOf<AEnemyBase>, int> EnemyMap = WaveData.EnemyData;
	bIsFinalWave = WaveData.bIsFinalWave;

	for (auto itr = EnemyMap.CreateIterator(); itr; ++itr)
	{
		for (int i = 0; i < itr.Value(); ++i)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(itr.Key().Get(), GetActorLocation()
				+ FVector(FMath::FRandRange(-100.f, 100.f), FMath::FRandRange(-100.f, 100.f), 0), FRotator::ZeroRotator,
				Params);
			ThisWaveEnemy.Add(SpawnedEnemy);
			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AStageManager::HandleEnemyDestroyed);
		}
	}

}

// Sets default values
AStageManager::AStageManager(): RootSceneComp(CreateDefaultSubobject<USceneComponent>(TEXT("RootScene")))
{
	SetRootComponent(RootSceneComp);
}

// Called when the game starts or when spawned
void AStageManager::BeginPlay()
{
	Super::BeginPlay();

	if (StageDataTableRow.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stage Data is null. Cannot spawn any enemies."))
		return;
	}

	StageData = *StageDataTableRow.GetRow<FStageData>(TEXT("Context"));

	SpawnWave(StageData.Waves[0]);
}