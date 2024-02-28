﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"


struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class AProjectEscapePlayer;
class UPhysicsHandleComp;
class APickableActor;
class UParticleSystem;
class AEnemyBase;
class AProjectEscapePlayerController;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActionGrab;


	UPROPERTY( EditDefaultsOnly, Category="Input" )
	UInputAction* InputActionQSkill;

	UPROPERTY()
	AProjectEscapePlayer* Player;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY( EditDefaultsOnly, Category="Weapon" )
	float MaxDistanceToGrab=10000000.0f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	UPhysicsHandleComp* HandleObject;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float NewInterpolSpeed= 6.0f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	bool bIsGrabbing = false;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	bool bIsPushing = false;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	bool bIsPulling = false;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float NewAngle = 1.0f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float RadiusDetection = 330.f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float RadiusDetectionEnemy=1000.f;

	UPROPERTY( EditDefaultsOnly )
	float EnemyHPMaxDistance = 100000.f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float RadiusDetectionHPBar = 1000.f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float RotSpeed = 10000.0f;

	//UPROPERTY( EditDefaultsOnly, Category="Grab" )
	FHitResult PickUpActorResult;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	APickableActor* ObjectInHand;

	UPROPERTY( EditDefaultsOnly, Category="Throw" )
	float ThrowingPower = 20000.0f;

	UPROPERTY( EditAnywhere, Category="QSkill" )
	class UNiagaraSystem* QExplosionEffect;

	//UPROPERTY( EditDefaultsOnly, Category="Throw" )
	//FVector EnemyLoc;


	UPROPERTY( EditDefaultsOnly, Category="Throw" )
	FVector ThrowingLoc;


	UPROPERTY( EditDefaultsOnly )
	int32 QSkillMaxCoolTime = 6;

	UPROPERTY( EditDefaultsOnly )
	int32 ESkillMaxCoolTime = 5;


	UPROPERTY( EditDefaultsOnly )
	int32 QSkillCurrentCoolTime=6;

	UPROPERTY( EditDefaultsOnly )
	int32 ESkillCurrentCoolTime=5;

	//UPROPERTY( EditDefaultsOnly, Category="QSkill" )
	//UParticleSystem* QExplosionEffect;

	UPROPERTY( EditAnywhere )
	AProjectEscapePlayerController* PC;


	UPROPERTY( EditAnywhere )
	//TArray<AActor*> OtherEnemies;
	TArray<AEnemyBase*> OtherEnemies;

	FTimerHandle ESkillCountDownHandle;
	FTimerHandle QSkillCountDownHandle;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void InitializeComponent() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

	void GrabObject();

	void ReleaseObject();

	void ActionQSkill();

	void SphereGrabObject();

	void QSkillAdvanceTimer();
	void ESkillAdvanceTimer();

	void ESkillUpdateTimerDisplay();

	void QSkillUpdateTimerDisplay();
	virtual void Deactivate() override;
};
