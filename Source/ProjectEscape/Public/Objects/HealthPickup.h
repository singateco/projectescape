﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class PROJECTESCAPE_API AHealthPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* MagnetBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealValue {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagnetDuration {1.0f};

	UPROPERTY()
	class UFCTweenUObject* MagnetTweenObj;

	UPROPERTY()
	FVector MagnetStartVector;

	UPROPERTY()
	class AProjectEscapePlayer* PlayerTarget;
	
protected:
	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnMagnetBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
