// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


struct FInputActionInstance;
struct FInputActionValue;
class AProjectEscapePlayer;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// #################################
	// ########### FUNCTIONS ###########
	// #################################
	
	UMoveComponent();

	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// Handles jumping.
	void HandleJump(const FInputActionInstance& InputActionInstance);

	void CheckForGroundWhileFlying();
	void FallDownWhileFlying();

	// #################################
	// ########### PROPERTIES ##########
	// #################################

	UPROPERTY()
	AProjectEscapePlayer* Player;
	
	// 떠다닐 시 떠다니는 모드 해제를 위해 지면을 체크하는 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GroundCheckDistance {-145.f};

	// 떠다닐시 밑으로 떨어지는 힘
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DownwardForce {5000.f};
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
