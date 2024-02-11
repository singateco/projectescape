// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectEscape/Public/Player/MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"


// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{

	// Input Action 설정.
	
	const static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionObjectFinder
	{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'")};

	if (JumpActionObjectFinder.Succeeded())
	{
		JumpAction = JumpActionObjectFinder.Object;
	}
	
	const static ConstructorHelpers::FObjectFinder<UInputAction> LookActionObjectFinder
	{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'")};

	if (LookActionObjectFinder.Succeeded())
	{
		LookAction = LookActionObjectFinder.Object;
	}

	const static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionObjectFinder
	{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'")};

	if (MoveActionObjectFinder.Succeeded())
	{
		MoveAction = MoveActionObjectFinder.Object;
	}

	const static ConstructorHelpers::FObjectFinder<UInputAction> DashActionObjectFinder
	{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Dash.IA_Dash'")};

	if (DashActionObjectFinder.Succeeded())
	{
		DashAction = DashActionObjectFinder.Object;
	}
	
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UMoveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapePlayer>();
	check(Player);

	Player->LandedDelegate.AddDynamic(this, &UMoveComponent::HandleLanding);
}

void UMoveComponent::DebugShowStamina()
{
	const FString StaminaString = FString::Printf(TEXT("Stamina: %.f/%.f"), Stamina, MaxStamina);

	DrawDebugString(GetWorld(),
		Player->GetActorLocation(),
		StaminaString,
		nullptr,
		FColor::White,
		0.f,
		true,
		1);
}


void UMoveComponent::PlayDashAnim()
{
	// Calculate which animations to play.

	float DotForward = FVector::DotProduct(MoveVector, Player->GetActorForwardVector());
	float DotRight = FVector::DotProduct(MoveVector, Player->GetActorRightVector());
	
	UAnimMontage* AnimMontageToPlay;
	
	if (DotForward >= 0.5)
	{
		// Forward
		AnimMontageToPlay = DashForwardAnimMontage;
	}
	else if (DotForward <= -0.5)
	{
		// Backwards
		AnimMontageToPlay = DashBackwardsAnimMontage;
	}
	else if (DotRight >= 0.5)
	{
		// Right
		AnimMontageToPlay = DashRightAnimMontage;
	}
	else if (DotRight <= -0.5)
	{
		//Left
		AnimMontageToPlay = DashLeftAnimMontage;
	}
	else
	{
		// Forward
		AnimMontageToPlay = DashForwardAnimMontage;
	}

	UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AnimMontageToPlay);
}

// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Player->DefaultMappingContext, 0);
		}
	}

	// 스태미나 적용
	Stamina = MaxStamina;
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ManageFlying(DeltaTime);
	RecoverStamina(DeltaTime);
	DebugShowStamina();
}

void UMoveComponent::CheckForGroundWhileFlying()
{
	FVector End = Player->GetActorLocation() + Player->GetActorUpVector() * GroundCheckDistance;

	// Debug line
	DrawDebugLine(GetWorld(), Player->GetActorLocation(), End, FColor::Red);

	FHitResult Result;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Result,
		Player->GetActorLocation(),
		End,
		ECC_Visibility);

	if (bHit)
	{
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

void UMoveComponent::FallDownWhileFlying()
{
	Player->GetCharacterMovement()->AddForce(FVector(0, 0, -1) * DownwardForce);
}

void UMoveComponent::RecoverStamina(const float DeltaTime)
{
	if (!bCanRecoverStamina)
	{
		return;
	}

	Stamina = FMath::Clamp(Stamina + (StaminaRecoveryPerSecond * DeltaTime), 0, MaxStamina);
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{

	// Jumping
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UMoveComponent::HandleJump);
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, Player, &ACharacter::StopJumping);

	// Moving
	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	
	// Looking
	PlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UMoveComponent::Look);

	// 대시
	PlayerInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &UMoveComponent::Dash);
}


void UMoveComponent::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	

	if (Player->Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Player->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		Player->AddMovementInput(ForwardDirection, MovementVector.Y);
		Player->AddMovementInput(RightDirection, MovementVector.X);

		MoveVector = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
		MoveVector.Normalize();
	}
}

void UMoveComponent::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Player->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		Player->AddControllerYawInput(LookAxisVector.X);
		Player->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void UMoveComponent::HandleJump(const FInputActionInstance& InputActionInstance)
{

	Player->Jump();
	
	if (Player->GetCharacterMovement()->IsMovingOnGround())
	{
		return;
	}
	
	if (Player->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		FVector NowVelocity = Player->GetVelocity();
		Player->GetCharacterMovement()->Velocity = FVector(NowVelocity.X, NowVelocity.Y, 0);
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	else
	{
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	}
}

void UMoveComponent::HandleLanding(const FHitResult& Hit)
{
	bCanRecoverStamina = true;
}

void UMoveComponent::Dash(const FInputActionInstance& InputActionInstance)
{
	if (Stamina < DashStamina)
	{
		return;
	}

	Stamina -= DashStamina;
	Player->GetCharacterMovement()->AddImpulse(MoveVector * DashForce, true);
	PlayDashAnim();
}

void UMoveComponent::ManageFlying(const float DeltaTime)
{
	if (Player->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		return;
	}

	bCanRecoverStamina = false;
	Stamina -= StaminaRecoveryPerSecond * DeltaTime;

	if (Stamina <= 0.f)
	{
		Player->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		return;
	}
		
	CheckForGroundWhileFlying();
	FallDownWhileFlying();
}
