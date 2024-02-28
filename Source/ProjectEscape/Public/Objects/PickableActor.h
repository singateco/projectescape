// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableActor.generated.h"


class UParticleSystem;
class UStaticMeshComponent;
class USphereComponent;
class UNiagaraSystem;
class AProjectEscapePlayer;

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

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float SphereRadius = 2000.f;

	UPROPERTY( EditDefaultsOnly)
	//UParticleSystem* GunEffect;
	UNiagaraSystem* ExplosionObjEffect;

	UPROPERTY( EditDefaultsOnly )
	//UParticleSystem* GunEffect;
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY( EditDefaultsOnly )
	AProjectEscapePlayer* Player;

	UPROPERTY( EditDefaultsOnly )
	float EmitterScaleValue = 7;

	UPROPERTY( EditDefaultsOnly )
	FVector ExplosionScale = FVector( 1 );

	UPROPERTY( EditDefaultsOnly, Category="Pistol" )
	UNiagaraSystem* BloodEffect;

	FString txt1;
	FString txt2;
	FString txt3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetExplosionRadius();
	
	UFUNCTION()
	void OnCompHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
