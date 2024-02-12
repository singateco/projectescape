﻿// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"
#include "CharacterBase.h"
#include "ProjectEscapePlayer.generated.h"

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
	
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	
	AProjectEscapePlayer();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

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

};

