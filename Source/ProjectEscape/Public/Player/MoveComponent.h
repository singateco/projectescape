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

	void DebugShowStamina();
	
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

	UFUNCTION()
	void HandleLanding(const FHitResult& Hit);

	// 대시
	void Dash(const FInputActionInstance& InputActionInstance);

	// 비행
	void ManageFlying(const float DeltaTime);
	void CheckForGroundWhileFlying();
	void FallDownWhileFlying();

	// 스태미나
	void RecoverStamina(const float DeltaTime);


	// #################################
	// ########### PROPERTIES ##########
	// #################################

	// 마지막으로 받은 X,Y 입력값. 대시시 사용함.
	UPROPERTY(BlueprintReadWrite)
	FVector MoveVector;

	// 플레이어.
	UPROPERTY()
	AProjectEscapePlayer* Player;


	// =============== 비행 ===============
	
	// 떠다닐 시 떠다니는 모드 해제를 위해 지면을 체크하는 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Flying")
	float GroundCheckDistance {-145.f};

	// 떠다닐시 밑으로 떨어지는 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Flying")
	float DownwardForce {5000.f};

	// 비행시 1초당 사용하는 스태미나.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Flying")
	float FlyingStaminaPerSecond {10.f};

	// =============== 대시 ===============
	
	// 대시시 가하는 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Dashing")
	float DashForce {1000.f};

	// 대시 사용시 사용하는 스태미나.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Dashing")
	float DashStamina {25.f};

	// ============= 스태미나 ===============
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina {100.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float Stamina;

	// 1초에 회복하는 스태미나의 양.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRecoveryPerSecond {20.f};

	UPROPERTY(VisibleAnywhere, Category = "Stamina")
	bool bCanRecoverStamina {true};

	// ############### 애니메이션 ################
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DashForwardAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DashBackwardsAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DashLeftAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* DashRightAnimMontage;

	UFUNCTION()
	void PlayDashAnim();

private:
	// Input Actions
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** 대시 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
