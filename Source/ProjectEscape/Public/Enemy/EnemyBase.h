// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	
	AEnemyBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// --------------------- Variable ---------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnemyBaseFSM* EnemyBaseFSM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* EnemyHPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* BulletREF;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP {1};
	
	// --------------------- Function ---------------------------

	UFUNCTION(BlueprintCallable)
	void DamageProcess(float DamageValue);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPawnSensingComponent* EnemyPawnSensing;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool bCanSeePlayer;

	// --------------------- Function ---------------------------
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

};
