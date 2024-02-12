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

	float CurrentTime;

	UPROPERTY(EditAnywhere)
	float AttackTime = 1;

	float AttackDistance;
	FVector RandomLocation;

	int32 HP;
	int32 MaxHP;

	// --------------------- Function ---------------------------

	void SetState(EEnemyState Next);

	void TickIdle();
	virtual void TickMove();
	virtual void TickAttack();
	void TickDamage();
	void TickDie();


	void OnTakeDamage(int32 Damage);
	void UpdateHP(int32 NewHP);

	//bool UpdateRandomLocation(FVector origin, float radius, FVector& outLocation);
};
