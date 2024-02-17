// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectEscapeAnimInstance.generated.h"

/**
 * 
 */

class AProjectEscapePlayer;
class UAnimMontag;

UCLASS()
class PROJECTESCAPE_API UProjectEscapeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()

	const AProjectEscapePlayer* Player;

	////Normal Gun Reload
	//UPROPERTY( EditDefaultsOnly, Category="MyAnimSettings" )
	//UAnimMontage* ReloadMontage;

	void PlayReloadAnimation();

	//재장전 끝났을때 호출될 Modify 이벤트 함수
	UFUNCTION()
	void AnimNotify_AN_Reload_C();


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	bool bHasFiring {false};

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	bool bIsDashing {false};
};
