// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

UCLASS()
class PROJECTESCAPE_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereComponentBeginHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* EnemyBulletMesh;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY( EditAnywhere )
	float BulletDamage = 2.0f;

	UPROPERTY( EditAnywhere, Category = Effect)
	UMaterialInterface* BulletDecalWall;

	UPROPERTY( EditAnywhere, Category=Effect )
	UMaterialInterface* BulletDecalBlood;

	UPROPERTY( EditAnywhere, Category=Effect )
	class UNiagaraSystem* BulletImpactWall;

	UPROPERTY( EditAnywhere, Category=Effect )
	UNiagaraSystem* BulletImpactBlood;

	UPROPERTY( EditAnywhere, Category=Effect )
	USoundBase* BulletHitSoundWall;

	UPROPERTY( EditAnywhere, Category=Effect )
	USoundBase* BulletHitSoundBlood;
};
