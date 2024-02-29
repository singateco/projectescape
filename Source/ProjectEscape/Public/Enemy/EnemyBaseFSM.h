// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBaseFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	 UMETA(DisplayName = "Idle"),
	Move	 UMETA(DisplayName = "Move"),
	Attack	 UMETA(DisplayName = "Attack"),
	Grenade UMETA(DisplayNae = "Grenade"),
	Damage	UMETA(DisplayName = "Damage"),
	Die		UMETA(DisplayName = "Die"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UEnemyBaseFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBaseFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --------------------- Variable ---------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState State;

	UPROPERTY()
	class AEnemyBase* Enemy;

	UPROPERTY()
	class AProjectEscapePlayer* Player;

	UPROPERTY()
	class UEnemyAnimInstance* EnemyAnim;

	UPROPERTY()
	class AAIController* Ai;

	float CurrentTime = 0;

	float WidgetTime = 1.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	float MinAttackTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	float MaxAttackTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	float AttackTime;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	int32 Accuracy;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	float Spread;

	float DamageTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyState" )
	float AttackDistance;

	float DieTime = 6.0f;
	FVector RandomLocation;
	float EnemyAcceptanceRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanSeePlayer = false;

	UPROPERTY( EditAnywhere )
	UParticleSystem* MuzzleFlash;

	UPROPERTY( EditAnywhere )
	USoundBase* AttackSound;
	// --------------------- Function ---------------------------

	void SetState(EEnemyState Next);

	void TickIdle();
	virtual void TickMove();
	virtual void TickAttack();
	virtual void TickDamage();
	virtual void TickDie();

	void OnChangeMoveState();
	void OnChangeIdleState();

	UFUNCTION()
	virtual void OnTakeDamage(float Damage_Unused);

	UFUNCTION()
	void OnDying();

	bool UpdateRandomLocation(FVector origin, float radius, FVector& outLocation);
};
