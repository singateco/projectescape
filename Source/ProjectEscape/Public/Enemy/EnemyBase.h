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
	explicit AEnemyBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick( float DeltaSeconds ) override;
	
	virtual void PreInitializeComponents() override;

	// --------------------- Variable ---------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyStatsComponent* EnemyStatsComponent;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
		class UWidgetComponent* EnemyHPComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnemyHealthBar* EnemyHealthBarWidget;

	UPROPERTY(EditDefaultsOnly, meta = (Class))
	TSubclassOf<UUserWidget> DamageNumberWidgetClass;

	UPROPERTY( EditDefaultsOnly )
		USkeletalMeshComponent* GunMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly)
		class UEnemyBaseFSM* EnemyBaseFSM;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
		class UNavigationInvokerComponent* NavComponent;

	// --------------------- Function ---------------------------

	UFUNCTION()
	void DisplayDamageNumber(const float DamageToDisplay);
};
