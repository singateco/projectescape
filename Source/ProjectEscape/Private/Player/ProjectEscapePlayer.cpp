// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/ProjectEscapePlayer.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MoveComponent.h"
#include "Player/FireComponent.h"
#include "Player/GrabComponent.h"
#include "Player/PhysicsHandleComp.h"
#include "Player/PlayerStatsComponent.h"
#include "ProjectEscape/PEGameplayTags.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectEscapePlayer

AProjectEscapePlayer::AProjectEscapePlayer(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerStatsComponent>(TEXT("Stats Component")))
{
	const static ConstructorHelpers::FObjectFinder<UInputAction> LookActionObjectFinder
		{TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'")};

	if (LookActionObjectFinder.Succeeded())
	{
		LookAction = LookActionObjectFinder.Object;
	}
	
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	// 플레이어 마우스 좌우 회전 따라오게 함
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	// Add Custom Components
	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	FireComponent = CreateDefaultSubobject<UFireComponent>(TEXT("Fire Component"));
	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComp>( TEXT( "PysicsHandleComponent" ) );

	PlayerStatsComponent = Cast<UPlayerStatsComponent>(StatsComponent);

	// Set MaxHP
	MaxHP = 30;
}

void AProjectEscapePlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (PlayerStatsComponent)
	{
		PlayerStatsComponent->OnHPReachedZero.AddUniqueDynamic(this, &AProjectEscapePlayer::Die);
	}

	
}

void AProjectEscapePlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectEscapePlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProjectEscapePlayer::Die()
{
	bUseControllerRotationYaw = false;
	
	MoveComponent->Deactivate();
	FireComponent->Deactivate();
	GrabComponent->Deactivate();
	
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	AnimInst->Montage_Play(DyingAnimMontage);
}

void AProjectEscapePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		MoveComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		FireComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		GrabComponent->SetupPlayerInputComponent(EnhancedInputComponent);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectEscapePlayer::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

