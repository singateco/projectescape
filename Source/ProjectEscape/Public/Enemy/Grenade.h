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

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	UStaticMeshComponent* GrenadeMesh;

	UPROPERTY( EditAnywhere, Category = Effect )
	class UNiagaraSystem* ExplosionEffect;

	UPROPERTY( EditAnywhere, Category = Effect )
	USoundBase* ExplosionSound;

	UPROPERTY( EditAnywhere, Category = Effect )
	UMaterialInterface* GrenadeDecal;

	UPROPERTY( EditAnywhere, Category=Effect )
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY( EditAnywhere )
	float ShakeRadius = 2000.0f;

	UPROPERTY( EditAnywhere )
	float ExplosionTime = 4.0f;

	UPROPERTY( EditAnywhere )
	float SphereRadius = 500.0f;

	UPROPERTY( EditAnywhere )
	float ExplosionEffectSize = 2.0f;

	UFUNCTION(BlueprintCallable)
	void Explosion();

	virtual void Explosion(const FHitResult& Hit);

	UFUNCTION()
	virtual void OnMeshBeginHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
