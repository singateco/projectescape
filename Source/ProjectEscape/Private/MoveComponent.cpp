// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectEscape/Public/MoveComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscape/ProjectEscapeCharacter.h"


// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}

void UMoveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapeCharacter>();
	check(Player);
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
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckForGroundWhileFlying();
	FallDownWhileFlying();
}

void UMoveComponent::CheckForGroundWhileFlying()
{
	if (Player->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		return;
	}

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
	if (Player->GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		return;
	}

	Player->GetCharacterMovement()->AddForce(FVector(0, 0, -1) * DownwardForce);
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
