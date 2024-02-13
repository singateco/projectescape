// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"


struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class AProjectEscapePlayer;
class UPhysicsHandleComp;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActionGrab;

	UPROPERTY()
	AProjectEscapePlayer* Player;


	UPROPERTY( EditDefaultsOnly, Category="Weapon" )
	float MaxDistanceToGrab=100000.0f;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	UPhysicsHandleComp* HandleObject;

	UPROPERTY( EditDefaultsOnly, Category="Grab" )
	float NewInterpolSpeed= 0.0f;

	bool bIsGrabbing = false;

	float NewAngle = 1.0f;

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
};
