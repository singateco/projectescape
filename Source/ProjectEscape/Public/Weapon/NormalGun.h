// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NormalGun.generated.h"

class AProjectEscapePlayer;

UCLASS()
class PROJECTESCAPE_API ANormalGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalGun();

	UPROPERTY()
	AProjectEscapePlayer* Player;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* NormalGunMesh;


	/* Normal Gun */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class USceneComponent* GunComp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
