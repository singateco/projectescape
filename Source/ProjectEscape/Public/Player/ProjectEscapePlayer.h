// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.h"
#include "ProjectEscapePlayer.generated.h"

class UNiagaraComponent;
class UMoveComponent;
class UFireComponent;
class UGrabComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPhysicsHandleComp;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProjectEscapePlayer : public ACharacterBase
{
	GENERATED_BODY()

public:

	AProjectEscapePlayer(const FObjectInitializer& ObjectInitializer);
	
	// #################################
	// ########### PROPERTIES ##########
	// #################################
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMoveComponent* MoveComponent;

	// Fire Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFireComponent* FireComponent;

	// Grab Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGrabComponent* GrabComponent;

	// Grab Component
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	UPhysicsHandleComp* PhysicsHandleComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	class UPlayerStatsComponent* PlayerStatsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* QShieldEffect;
	
	// #################################
	// ########### FUNCTIONS ###########
	// #################################

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Die();

	UPROPERTY(VisibleAnywhere)
	bool bIsDead {false};


	UPROPERTY( VisibleAnywhere )
	bool IsReloading=false;

protected:
	// #################################
	// ########### FUNCTIONS ###########
	// #################################

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DyingAnimMontage;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void PlayHitReactAnim(const FHitResult& HitResult);

	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent)
	UAnimMontage* SelectHitMontage(FVector HitNormal, AActor* HitActor);

	virtual void Tick(float DeltaSeconds) override;

private:
	// #################################
	// ########### PROPERTIES ##########
	// #################################
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
};

