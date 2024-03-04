// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageManager.h"

#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/EnemySpawnableArea.h"
#include "System/ProjectEscapePlayerController.h"
#include "UI/MainUI.h"
#include "UI/ObjectiveWidget.h"

void AStageManager::FirstWaveSpawn()
{
	if (const auto PC = Cast<AProjectEscapePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (PC->InGameWIdget)
		{
			PC->InGameWIdget->Objective->Init(this);
		}
	}
	
	SpawnWave(StageData.Waves[0]);
}

void AStageManager::HandleEnemyDestroyed(AEnemyBase* DestroyedActor)
{
	DestroyedActor->OnEnemyDied.RemoveDynamic(this, &AStageManager::HandleEnemyDestroyed);
	ThisWaveEnemy.RemoveSwap(DestroyedActor);
	
	if (ThisWaveEnemy.IsEmpty())
	{
		if (bIsFinalWave)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stage Won!"))
			OnStageFinished.Broadcast();

			if (GetWorld()->GetTimerManager().IsTimerActive(BonusObjectiveHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(BonusObjectiveHandle);
			}
			return;
		}

		OnWaveFinished.Broadcast(StageData.Waves[CurrentWaveIndex], StageData.Waves[++CurrentWaveIndex]);
		TWeakObjectPtr<AStageManager> WeakThis = this;
		if (StageData.Waves.Num() > CurrentWaveIndex)
		{
			FTimerHandle NextWaveHandle;
			GetWorld()->GetTimerManager().SetTimer(NextWaveHandle,
				FTimerDelegate::CreateLambda([WeakThis] {
				if (WeakThis.IsValid())
				{
					WeakThis->SpawnWave(WeakThis->StageData.Waves[WeakThis->CurrentWaveIndex]);
				}
			}),
			BetweenWavesDelaySeconds,
			false
			);
		}
	}
}

void AStageManager::BonusObjectiveTimeoutFailed()
{
	OnBonusObjectiveTimeoutFailed.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(BonusObjectiveHandle);
	bBonusReward = false;
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
			SpawnedEnemy->OnEnemyDied.AddUniqueDynamic(this, &AStageManager::HandleEnemyDestroyed);
		}
	}


	bBonusReward = true;
	
	GetWorld()->GetTimerManager().ClearTimer(BonusObjectiveHandle);

	GetWorld()->GetTimerManager().SetTimer(BonusObjectiveHandle,
		FTimerDelegate::CreateUObject(this, &AStageManager::BonusObjectiveTimeoutFailed),
		BonusObjectiveSeconds,
		false
	);

	OnWaveStarted.Broadcast(WaveData, ThisWaveEnemy);
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

	FTimerHandle FirstSpawn;
	GetWorld()->GetTimerManager().SetTimer(FirstSpawn,
		FTimerDelegate::CreateUObject(this, &AStageManager::FirstWaveSpawn),
		1.0f,
		false
	);
}