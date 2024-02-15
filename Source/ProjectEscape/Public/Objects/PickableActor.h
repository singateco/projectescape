// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"


class UParticleSystem;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class PROJECTESCAPE_API APickableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableActor();

	UPROPERTY( EditAnywhere )
	USphereComponent* CollisionComp;

	UPROPERTY( EditAnywhere )
	UStaticMeshComponent* MeshComp;

	UPROPERTY( EditAnywhere )
	float SphereRadius = 500.0f;

	UPROPERTY( EditDefaultsOnly)
	UParticleSystem* GunEffect;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCompHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
