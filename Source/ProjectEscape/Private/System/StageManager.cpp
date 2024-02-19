// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageManager.h"

#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/EnemySpawnableArea.h"

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
			OnStageFinished.Broadcast();
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

			// Get a random location from spawnable areas.
			TArray<AActor*> SpawnableFinder;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnableArea::StaticClass(),SpawnableFinder);

			// If there are no spawnable areas, use the manager location.
			FVector RandomLocation = GetActorLocation();
			
			if (!SpawnableFinder.IsEmpty())
			{
				UBoxComponent* SpawnBox = Cast<AEnemySpawnableArea>(
					SpawnableFinder[FMath::RandRange(0, SpawnableFinder.Num() - 1)])->SpawnBoxExtent;

				FVector Origin = SpawnBox->Bounds.Origin;
				FVector Extent = SpawnBox->Bounds.BoxExtent;

				RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
			}
			
			AEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(
				itr.Key().Get(),
				RandomLocation,
				FRotator::ZeroRotator,
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