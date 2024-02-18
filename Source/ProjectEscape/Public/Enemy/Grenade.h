// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class PROJECTESCAPE_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY( EditDefaultsOnly )
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY( EditAnywhere, Category = Effect )
	UParticleSystem* ExplosionEffect;

	UPROPERTY( EditAnywhere, Category = Effect )
	USoundBase* ExplosionSound;

	UPROPERTY( EditAnywhere )
	float ExplosionTime = 4.0f;

	UPROPERTY( EditAnywhere )
	float SphereRadius = 500.0f;

	void Explosion();
};
