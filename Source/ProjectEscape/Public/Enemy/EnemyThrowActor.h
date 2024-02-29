// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grenade.h"
#include "Enemy/Rocket.h"
#include "EnemyThrowActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEnemyThrowActor : public AGrenade
{
	GENERATED_BODY()

	AEnemyThrowActor();

	void Explosion(const FHitResult& Hit) override;

	void OnMeshBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
