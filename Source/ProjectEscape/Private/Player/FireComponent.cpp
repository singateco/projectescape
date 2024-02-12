// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FireComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontageFinder {TEXT("/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Pistol_Fire.AM_MM_Pistol_Fire'")};
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

	// ...
	check(NormalGunClass);
	NormalGun = GetWorld()->SpawnActor<ANormalGun>(NormalGunClass);
	AttachPistol();
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
	PlayerInputComponent->BindAction(ActionFire, ETriggerEvent::Started, this, &UFireComponent::NormalGunFire);
}

void UFireComponent::NormalGunFire()
{
	if (bHasPistol == false ) {
		return;
	}

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
		FVector EndPos2= HitInfo1.Location;


		FHitResult HitInfo2;
		FCollisionQueryParams Params2;
		Params2.AddIgnoredActor( Player );
		bool bHit2 = GetWorld()->LineTraceSingleByChannel( HitInfo2, StartPos2, EndPos2, ECC_Visibility, Params2 );

		if( bHit2 )
		{
			//DrawDebugLine( GetWorld(), StartPos2, EndPos2, FColor::Red, true );
			//DrawDebugBox(GetWorld(), HitInfo2.Location, FVector(5), FColor::Red, false, 5.f, 0, 3);
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, HitInfo2.Location, FRotator() );
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, EndPos2, FRotator() );
		}
		
		HandleFireAnimation();
			
		//if (Enemy) {
		//	Enemy->DamageProcess();
		//}
	}
	//auto Anim = Cast<UProjectEscapeAnimInstance>(GetMesh()->GetAnimInstance());
	//Anim->PlayerFireAnimation();
}

void UFireComponent::AttachPistol()
{
	//Bullet ReLoad Animation
	bHasPistol = true;
	NormalGun->SetActorHiddenInGame(false);
	NormalGun->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunPosition"));
}


void UFireComponent::DetachPistol()
{
	bHasPistol = false;
	NormalGun->SetActorHiddenInGame(true);
	//NormalGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//NormalGun->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}