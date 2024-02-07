// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBase();


	// --------------------- Variable ---------------------------
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* EnemySkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnemyBaseFSM* EnemyBaseFSM;





	// --------------------- Function ---------------------------

	

};
