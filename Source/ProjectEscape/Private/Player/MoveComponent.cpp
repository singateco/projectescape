// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectEscape/Public/Player/MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "PECharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectEscape/PEGameplayTags.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "UI/PlayerStaminaUI.h"


// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
	:
	DashLine(CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dash Line Effect"))),
	FallSmoke(CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fall Smoke Effect")))
{
	// Input Action 설정.

	const static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionObjectFinder
		{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'")};

	if (JumpActionObjectFinder.Succeeded())
	{
		JumpAction = JumpActionObjectFinder.Object;
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

	const static ConstructorHelpers::FObjectFinder<UInputAction> FlyActionObjectFinder
		{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Fly.IA_Fly'")};

	if (FlyActionObjectFinder.Succeeded())
	{
		FlyAction = FlyActionObjectFinder.Object;
	}
	
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UMoveComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapePlayer>();
	check(Player);
	
	CharacterMovementComponent = Cast<UPECharacterMovementComponent>(Player->GetCharacterMovement());

	Player->LandedDelegate.AddDynamic(this, &UMoveComponent::HandleLanding);
	Player->GetMesh()->GetAnimInstance()->OnMontageEnded.AddUniqueDynamic(this, &UMoveComponent::HandleOnMontageEnded);

	// 이펙트 설정
	DashLine->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	DashLine->SetRelativeLocation(FVector(0, 0, 20));
	DashLine->SetRelativeRotation(FRotator(0, 90, 0));
	FallSmoke->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void UMoveComponent::ShowDebugStat()
{
	FString DebugString = FString::Printf(TEXT("Stamina: %.f/%.f"), Stamina, MaxStamina);
	DebugString += FString::Printf(TEXT("\nVelocity: %.f"), CharacterMovementComponent->GetLastUpdateVelocity().Length());
	DebugString += FString::Printf(TEXT("\nMovement Mode: %s"), *CharacterMovementComponent->GetMovementName());
	
	DrawDebugString(GetWorld(),
		Player->GetActorLocation(),
		DebugString,
		nullptr,
		FColor::White,
		0.f,
		true,
		1);
}


void UMoveComponent::HandleOnMontageEnded(UAnimMontage* Montage, bool Interrupted)
{
	if (Montage == DashBackwardsAnimMontage ||
		Montage == DashForwardAnimMontage ||
		Montage == DashLeftAnimMontage ||
		Montage == DashRightAnimMontage)
	{
		Player->RemoveGameplayTag(PEGameplayTags::Status_IsDashing);
	}
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

void UMoveComponent::SetEffectState()
{
	// Set Dash Line state.
	if (CharacterMovementComponent->GetLastUpdateVelocity().Length() > DashLineShowVelocity ||
		CharacterMovementComponent->IsFlying() && CharacterMovementComponent->GetLastUpdateVelocity().Length() > DashLineShowVelocity / 2.f)
	{
		DashLine->Activate();
	}
	else
	{
		DashLine->Deactivate();
	}

	if (CharacterMovementComponent->IsFlying() ||
		CharacterMovementComponent->IsFalling())
	{
		FallSmoke->SetVisibility(true);
	}
	else
	{
		FallSmoke->SetVisibility(false);
	}
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

void UMoveComponent::Deactivate()
{
	Super::Deactivate();

	EnhancedInputComponent->ClearBindingsForObject(this);
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ManageFlying(DeltaTime);
	RecoverStamina(DeltaTime);
	SetEffectState();
	//ShowDebugStat();

	if (PlayerStaminaUI)
	{
		PlayerStaminaUI->UpdateStamina(MaxStamina, Stamina);
	}
}

void UMoveComponent::FlyButton(const FInputActionInstance& InputActionInstance)
{
	if (CharacterMovementComponent->MovementMode != MOVE_Flying && Stamina >= 0.02)
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Flying);
	}

	if (Stamina >= 0)
	{
		CharacterMovementComponent->AddForce(FVector(0, 0, UpwardForce));
		Stamina = FMath::Max(0, Stamina - UpwardStaminaPerSecond * GetWorld()->GetDeltaSeconds());
	}
}

void UMoveComponent::CheckForGroundWhileFlying()
{
	FVector End = Player->GetActorLocation() + Player->GetActorUpVector() * GroundCheckDistance;

	// Debug line
	// DrawDebugLine(GetWorld(), Player->GetActorLocation(), End, FColor::Red);

	FHitResult Result;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Result,
		Player->GetActorLocation(),
		End,
		ECC_Visibility);

	if (bHit)
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Falling);
	}
}

void UMoveComponent::FallDownWhileFlying()
{
	CharacterMovementComponent->AddForce(FVector(0, 0, -1) * DownwardForce);
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

	EnhancedInputComponent = PlayerInputComponent;

	// Jumping
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UMoveComponent::HandleJump);
	PlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, Player, &ACharacter::StopJumping);

	// Moving
	PlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UMoveComponent::Move);

	// Dashing
	PlayerInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &UMoveComponent::Dash);

	// Flying
	PlayerInputComponent->BindAction(FlyAction, ETriggerEvent::Triggered, this, &UMoveComponent::FlyButton);
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


void UMoveComponent::HandleJump(const FInputActionInstance& InputActionInstance)
{

	Player->Jump();
	
	if (CharacterMovementComponent->IsMovingOnGround())
	{
		return;
	}
	
	if (CharacterMovementComponent->MovementMode != MOVE_Flying && Stamina >= 0.05)
	{
		FVector NowVelocity = Player->GetVelocity();
		CharacterMovementComponent->Velocity = FVector(NowVelocity.X, NowVelocity.Y, 0);
		CharacterMovementComponent->SetMovementMode(MOVE_Flying);
	}
	else
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Falling);
	}
}

void UMoveComponent::HandleLanding(const FHitResult& Hit)
{
	bCanRecoverStamina = true;
}

void UMoveComponent::Dash(const FInputActionInstance& InputActionInstance)
{
	if (Stamina < DashStamina
		//|| bIsDashing
		)
	{
		return;
	}

	Stamina = FMath::Max(0, Stamina - DashStamina);
	Player->AddGameplayTag(PEGameplayTags::Status_IsDashing);

	if (CharacterMovementComponent->MovementMode == MOVE_Falling)
	{
		FVector NewVelocity = CharacterMovementComponent->GetLastUpdateVelocity();
		NewVelocity.Z = 0;
		CharacterMovementComponent->Velocity = NewVelocity;
		CharacterMovementComponent->UpdateComponentVelocity();
		CharacterMovementComponent->GravityScale = 0.05f;

		if (!DashGravityHandle.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(
			DashGravityHandle,
			FTimerDelegate::CreateLambda(
				[&]
				{
					CharacterMovementComponent->GravityScale = 1.f;
					DashGravityHandle.Invalidate();
				}),
			0.4f,
			false);
		}
	}
	
	CharacterMovementComponent->AddImpulse(MoveVector * DashForce, true);
	PlayDashAnim();
}

void UMoveComponent::ManageFlying(const float DeltaTime)
{
	if (CharacterMovementComponent->MovementMode != MOVE_Flying)
	{
		return;
	}

	bCanRecoverStamina = false;
	Stamina = FMath::Max(0, Stamina - FlyingStaminaPerSecond * DeltaTime);

	if (Stamina <= 0.f)
	{
		FallDownWhileFlying();
	}
	
	CheckForGroundWhileFlying();
}
