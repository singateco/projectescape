﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireComponent.generated.h"


struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class AProjectEscapePlayer;
class ANormalGun;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireComponent();



	UPROPERTY()
	AProjectEscapePlayer* Player;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ANormalGun* NormalGun;


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxDistanceToGun = 100000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bHasPistol = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActionFire;


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ANormalGun> NormalGunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	class UParticleSystem* GunEffect;

	virtual void InitializeComponent() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent) ;

	//void Fire();

	void NormalGunFire();

	void AttachPistol();

	void DetachPistol();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
