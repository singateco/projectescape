// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseFSM.h"
#include "Components/ActorComponent.h"
#include "RifleEnemyFSM.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API URifleEnemyFSM : public UEnemyBaseFSM
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URifleEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY( EditAnyWhere , BlueprintReadWrite, Category = "Rifle")
		TSubclassOf<class AEnemyBullet> EnemyBulletFactory;

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Category = "Rifle" )
		AEnemyBullet* EnemyBullet;

	UPROPERTY( EditAnywhere , BlueprintReadWrite, Category = "EnemyState" )
		int32 Accuracy;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
		float Spread;

	void TickAttack() override;
};
