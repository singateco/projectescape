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
	void OnSphereComponentBeginHit( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									   const FHitResult& SweepResult );

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
	UMaterialInterface* BulletDecal;

	UPROPERTY( EditAnywhere )
	class UNiagaraSystem* BulletImpact;
};
