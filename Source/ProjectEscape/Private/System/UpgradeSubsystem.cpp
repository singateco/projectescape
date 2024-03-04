// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UpgradeSubsystem.h"

#include "Player/PlayerStatsComponent.h"


UUpgradeSubsystem::UUpgradeSubsystem()
{
	static ConstructorHelpers::FObjectFinderOptional<UDataTable> DataTableFinder
	{TEXT("/Script/Engine.DataTable'/Game/Data/UpgradeData/UpgradeDataTable.UpgradeDataTable'")};

	UpgradeDataTable = DataTableFinder.Get();
}

void UUpgradeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UpgradeDataTable)
	{
		TArray<FUpgradeData*> OutRows;
		UpgradeDataTable->GetAllRows(TEXT("Getting upgrade datas from upgrade subsystem"), OutRows);

		for (const FUpgradeData* Data : OutRows)
		{
			StaticUpgradeDatas.Add(*Data);
		}
	}
}

FUpgradeData UUpgradeSubsystem::GetRandomUpgradeData() const
{
	return StaticUpgradeDatas.IsEmpty() ? FUpgradeData() : StaticUpgradeDatas[FMath::RandRange(0, StaticUpgradeDatas.Num() - 1)];
}

UUpgrade* UUpgradeSubsystem::MakeUpgradeFromStaticData(const FUpgradeData& Data)
{
	UUpgrade* NewUpgrade = NewObject<UUpgrade>(this, FName(FString::Printf(TEXT("Upgrade %d"), UpgradeCounter)
		+ " " + Data.Name.ToString()));
	NewUpgrade->StaticData = Data;
	NewUpgrade->UpgradeUID = UpgradeCounter;
	UpgradeCounter++;
	return NewUpgrade;
}

UUpgrade* UUpgradeSubsystem::GetRandomUpgrade()
{
	return MakeUpgradeFromStaticData(GetRandomUpgradeData());
}

void UUpgradeSubsystem::SaveUpgrade(UPlayerStatsComponent* PlayerStatsComponent)
{
	SavedUpgrades = PlayerStatsComponent->OwningUpgrades;
}

void UUpgradeSubsystem::LoadUpgrade(UPlayerStatsComponent* PlayerStatsComponent)
{
	for (auto Upgrade: SavedUpgrades)
	{
		PlayerStatsComponent->AddUpgrade(Upgrade);
	}
}
