// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

class UFCTweenUObject;
class UNiagaraComponent;
class AHealthPickup;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDied, class AEnemyBase*, Enemy);

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

	UPROPERTY()
	FEnemyDied OnEnemyDied;

	UPROPERTY()
	FVector2D CurrentLocationScreen;

	UPROPERTY()
	float CrosshairDist;
	
	UFUNCTION()
	virtual void ProcessDying();

	UPROPERTY()
	USoundBase* DyingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AHealthPickup> HealthPickupActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInstance* UIMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthDropChance {0.5f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyStatsComponent* EnemyStatsComponent;
	
	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	class UWidgetComponent* EnemyHPComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	UWidgetComponent* TargetUIComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyHealthBar* EnemyHealthBarWidget;

	UPROPERTY(EditDefaultsOnly, meta = (Class))
	TSubclassOf<UUserWidget> DamageNumberWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* SpawnEffectEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* SpawnEffectCircle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool PlaySpawnEffect {true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpawnEffectSeconds {4.0f};

	UPROPERTY()
	UFCTweenUObject* SpawnEffectTweenUObject;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	USkeletalMeshComponent* GunMesh;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly )
	class UBoxComponent* WeakPoint;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly)
	class UEnemyBaseFSM* EnemyBaseFSM;

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	class UNavigationInvokerComponent* NavComponent;

	UPROPERTY( EditAnywhere )
	float EnemyMaxSpeed = 250.0f;

	UFUNCTION( BlueprintImplementableEvent )
	void PlayAnimUITargetEnemy();

	UPROPERTY( EditDefaultsOnly )
	class UTargetUIComp* TargetComp;

	UPROPERTY()
	TSubclassOf<UUserWidget> TargetUIClass;

	UPROPERTY(EditAnywhere)
	FString EnemyName = TEXT("");

	// --------------------- Function ---------------------------
	UFUNCTION( BlueprintCallable )
	FString GetEnemyName();

	UFUNCTION()
	void DisplayDamageNumber(const float DamageToDisplay);
};
