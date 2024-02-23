// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rocket.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API ARocket : public AActor
{
	GENERATED_BODY()


public:
	ARocket();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY( EditDefaultsOnly )
	class USphereComponent* CollisionComp;

	UPROPERTY( EditDefaultsOnly )
	UStaticMeshComponent* RocketMesh;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY( EditAnywhere, Category = Effect )
	class UNiagaraSystem* ExplosionEffect;

	UPROPERTY( EditAnywhere, Category = Effect )
	USoundBase* ExplosionSound;

	UPROPERTY( EditAnywhere, Category = Effect )
	USoundBase* FlyingSound;

	/*UPROPERTY( EditAnywhere, Category=Effect )
	UMaterialInterface* RocketDecal;*/

	UPROPERTY( EditAnywhere )
	float SphereRadius = 300.0f;

	UPROPERTY( EditAnywhere )
	float ExplosionTime = 7.0f;

	UFUNCTION(BlueprintCallable)
	void Explosion();

	UFUNCTION()
	void OnComponentBeginHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

};
