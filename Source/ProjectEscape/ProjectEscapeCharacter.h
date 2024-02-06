// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Logging/LogMacros.h"
#include "Public/CharacterBase.h"
#include "ProjectEscapeCharacter.generated.h"

class UMoveComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AProjectEscapeCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// #################################
	// ########### PROPERTIES ##########
	// #################################

	// 떠다닐 시 떠다니는 모드 해제를 위해 지면을 체크하는 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GroundCheckDistance {-145.f};

	// 떠다닐시 밑으로 떨어지는 힘
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DownwardForce {5000.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMoveComponent* MoveComponent;
	
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	
	AProjectEscapeCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// #################################
	// ########### FUNCTIONS ###########
	// #################################

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// Handles jumping.
	void HandleJump(const FInputActionInstance& InputActionInstance);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	void CheckForGroundWhileFlying();
	void FallDownWhileFlying();

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
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
};

