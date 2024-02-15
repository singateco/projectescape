// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FireComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "Weapon/NormalGun.h"

// Sets default values for this component's properties
UFireComponent::UFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
	
	static ConstructorHelpers::FClassFinder<ANormalGun> NormalGunClassFinder {TEXT("Blueprint'/Game/Blueprints/BP_NormalGun.BP_NormalGun_C'")};
	if (NormalGunClassFinder.Succeeded())
	{
		NormalGunClass = NormalGunClassFinder.Class;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ActionFireFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Fire.IA_Fire'")};
	if (ActionFireFinder.Succeeded())
	{
		ActionFire = ActionFireFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireEffectFinder {TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'")};
	if (FireEffectFinder.Succeeded())
	{
		GunEffect = FireEffectFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontageFinder {TEXT("/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Pistol_DryFire.AM_MM_Pistol_DryFire'")};
	if (FireMontageFinder.Succeeded())
	{
		FireMontage = FireMontageFinder.Object;
	}
}


void  UFireComponent::HandleFireAnimation()
{
	bHasFired = true;
	UAnimInstance* AnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireMontage);
}

// Called when the game starts
void UFireComponent::BeginPlay()
{
	Super::BeginPlay();
	
	check(NormalGunClass);
	NormalGun = GetWorld()->SpawnActor<ANormalGun>(NormalGunClass);
	AttachPistol();
}

void UFireComponent::Deactivate()
{
	Super::Deactivate();

	EnhancedInputComponent->ClearBindingsForObject(this);
}

void UFireComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapePlayer>();
	check(Player);
}


// Called every frame
void UFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	bHasFired = false;
}

void UFireComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = PlayerInputComponent;
	PlayerInputComponent->BindAction(ActionFire, ETriggerEvent::Started, this, &UFireComponent::NormalGunFire);
	PlayerInputComponent->BindAction(ActionAimDownSight, ETriggerEvent::Started, this, &UFireComponent::StartAimDown);
	PlayerInputComponent->BindAction(ActionAimDownSight, ETriggerEvent::Completed, this, &UFireComponent::EndAimDown);
}

void UFireComponent::NormalGunFire()
{
	if (bHasPistol == false ) {
		return;
	}

	HandleFireAnimation();

	//FRotator GazeRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), Player->GetCameraBoom()->GetForwardVector() * MaxDistanceToGun);
	//Player->SetActorRotation(GazeRotation);

	// �ѽ��

	// 1. Collision Check - LineTrace 1st
	FHitResult HitInfo1;
	// 1) From Crosshair
	FVector StartPos1 = Player->GetFollowCamera()->GetComponentLocation();
	// 2) To End Point(Max Distance)
	FVector EndPos1 = Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector()*MaxDistanceToGun;

	FCollisionQueryParams Params1;
	Params1.AddIgnoredActor(Player);

	bool bHit1 = GetWorld()->LineTraceSingleByChannel(HitInfo1, StartPos1, EndPos1,ECC_Visibility, Params1);
	
	//DrawDebugLine( GetWorld(), StartPos1, EndPos1, FColor::Blue, true );
	if (bHit1) { 

		// 2. Collision Check - LineTrace 2nd
		// 1) From Muzzle
		FVector StartPos2=NormalGun->NormalGunMesh->GetSocketLocation( TEXT( "Muzzle" ) );
		// 2) To Collision Position 
		FVector EndPos2= HitInfo1.Location + Player->GetFollowCamera()->GetForwardVector() * 1;


		FHitResult HitInfo2;
		FCollisionQueryParams Params2;
		Params2.AddIgnoredActor( Player );
		bool bHit2 = GetWorld()->LineTraceSingleByChannel( HitInfo2, StartPos2, EndPos2, ECC_Visibility, Params2 );
		AEnemyBase* Enemy = nullptr;
		
		if( bHit2 )
		{
			//DrawDebugLine( GetWorld(), StartPos2, EndPos2, FColor::Red, true );
			//DrawDebugBox(GetWorld(), HitInfo2.Location, FVector(5), FColor::Red, false, 5.f, 0, 3);
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, HitInfo2.Location, FRotator() );
			Enemy = Cast<AEnemyBase>(HitInfo2.GetActor());
			//UE_LOG(LogTemp, Warning, TEXT("hit actor: %s"), *HitInfo2.GetActor()->GetActorNameOrLabel())
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, EndPos2, FRotator() );
		}
		
		if (Enemy)
		{
			Enemy->ProcessDamage(1);
		}
	}
}

void UFireComponent::AttachPistol()
{
	//Bullet ReLoad Animation
	bHasPistol = true;
	NormalGun->SetActorHiddenInGame(false);

	TArray<UActorComponent*> Comp = Player->GetComponentsByTag(USkeletalMeshComponent::StaticClass(), TEXT("Body"));
	USceneComponent* BodyComp = Cast<USceneComponent>(Comp[0]);
	NormalGun->AttachToComponent(BodyComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunPosition"));
}


void UFireComponent::DetachPistol()
{
	bHasPistol = false;
	NormalGun->SetActorHiddenInGame(true);
	//NormalGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//NormalGun->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}

void UFireComponent::StartAimDown(const FInputActionInstance& InputActionInstance)
{
	//Player->GetCameraBoom()->TargetArmLength = 100;
}

void UFireComponent::EndAimDown(const FInputActionInstance& InputActionInstance)
{
	//Player->GetCameraBoom()->TargetArmLength = 300;
}
