// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FireComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animations/ProjectEscapeAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/Debuff.h"
#include "Components/TextBlock.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyStatsComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Player/GrabComponent.h"
#include "Player/PlayerStatsComponent.h"
#include "ProjectEscape/PEGameplayTags.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "System/ProjectEscapePlayerController.h"
#include "UI/MainUI.h"
#include "Weapon/NormalGun.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "System/ProjectEscapePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ProfilingDebugging/CookStats.h"

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

	static const ConstructorHelpers::FObjectFinder<UInputAction> ActionFireFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Fire.IA_Fire'")};
	if (ActionFireFinder.Succeeded())
	{
		ActionFire = ActionFireFinder.Object;
	}

	//static const ConstructorHelpers::FObjectFinder<UParticleSystem> FireEffectFinder {TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'")};

	//Upgrade Gun Effect
	static const ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireUpgradeEffectFinder {TEXT("/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/ExplosionsMegaPack/Niagara/Flak/NS_Explosion_Flak_2.NS_Explosion_Flak_2'")};
	if ( FireUpgradeEffectFinder.Succeeded() )
	{
		GunUpgradeEffect=FireUpgradeEffectFinder.Object;
	}


	// Blood VFX
	static const ConstructorHelpers::FObjectFinder<UNiagaraSystem> BloodEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Large.OS_NS_Bullet_Hit_Large'" ) };
	if ( BloodEffectFinder.Succeeded())
	{
		BloodEffect =BloodEffectFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireEffectNoActorFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/BulletImpactVFX/BulletHitVFX_SFX/AParticleSystem_Niagara/PS_BulletHit_Concrete_N.PS_BulletHit_Concrete_N'") };

	if ( FireEffectNoActorFinder.Succeeded() )
	{
		GunEffectNoActor=FireEffectNoActorFinder.Object;
	}


	static const ConstructorHelpers::FObjectFinder<UMaterialInterface> UMaterialInterfaceFinder{ TEXT( "/Script/Engine.MaterialInstanceConstant'/Game/Resources/KDE/BulletImpactVFX/BulletHoleDecals/Material_Instances/Inst_BulletDecal_Concrete_01.Inst_BulletDecal_Concrete_01'" ) };

	if ( UMaterialInterfaceFinder.Succeeded() )
	{
		WallDecalEffect=UMaterialInterfaceFinder.Object;
	}




	static ConstructorHelpers::FObjectFinder<USoundBase> GunSoundFinder{ TEXT( "/Script/Engine.SoundWave'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Audio/GunFire/SW_GunFire_05.SW_GunFire_05'" ) };
	if ( GunSoundFinder.Succeeded() )
	{
		GunSoundClass = GunSoundFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ReloadSoundFinder( TEXT( "/Script/Engine.SoundWave'/Game/Resources/KDE/Sound/S_LPAMG_WEP_X13_Reload.S_LPAMG_WEP_X13_Reload'" ) );

	if ( ReloadSoundFinder.Succeeded() )
	{
		ReloadSoundClass=ReloadSoundFinder.Object;
	}
	
	static const ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleEffectFinder{ TEXT( "/Script/Engine.ParticleSystem'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/VFX/P_MuzzleFlash_02.P_MuzzleFlash_02'" ) }; 
	if ( MuzzleEffectFinder.Succeeded() )
	{
		MuzzleEffect=MuzzleEffectFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontageFinder {TEXT("/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Pistol_Fire.AM_MM_Pistol_Fire'")};
	if (FireMontageFinder.Succeeded())
	{
		FireMontage = FireMontageFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UInputAction> ReloadActionFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Reload.IA_Reload'")};
	if (ReloadActionFinder.Succeeded())
	{
		ActionReload = ReloadActionFinder.Object;	
	}
	
	static const ConstructorHelpers::FObjectFinder<UInputAction> AdsActionFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_ADS.IA_ADS'")};
	if (AdsActionFinder.Succeeded())
	{
		ActionAimDownSight = AdsActionFinder.Object;
	}
	
	static const ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontageFinder{ TEXT( "/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Pistol_Reload.AM_MM_Pistol_Reload'" ) };
	if ( ReloadMontageFinder.Succeeded() )
	{
		ReloadMontage=ReloadMontageFinder.Object;
	}

	
}


void  UFireComponent::HandleFireAnimation()
{
	bHasFired = true;
	UAnimInstance* AnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FireMontage, Player->PlayerStatsComponent->FireRate);
}



void UFireComponent::PlayReloadAnimation()
{
	UAnimInstance* AnimInstance = Cast<ACharacter>( GetOwner() )->GetMesh()->GetAnimInstance();
	if ( ReloadMontage ) {
		AnimInstance->Montage_Play(ReloadMontage, Player->PlayerStatsComponent->ReloadSpeedRate);
	}
}


// Called when the game starts
void UFireComponent::BeginPlay()
{
	Super::BeginPlay();
	
	check(NormalGunClass);
	NormalGun = GetWorld()->SpawnActor<ANormalGun>(NormalGunClass);
	AttachPistol();
	InitBullets();

	if (PC)
	{
		MainUI = PC->InGameWIdget;
	}
}

void UFireComponent::Deactivate()
{
	Super::Deactivate();

	SetComponentTickEnabled(false);
	EnhancedInputComponent->ClearBindingsForObject(this);
}

void UFireComponent::CheckIfShootCanHit()
{
	// 1) From Crosshair
	FVector StartPos1 = Player->GetFollowCamera()->GetComponentLocation();
	// 2) To End Point(Max Distance)
	FVector EndPos1 = Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGun;

	FCollisionQueryParams Params1;
	Params1.AddIgnoredActor(Player);

	bool bHit1 = GetWorld()->LineTraceSingleByChannel(HitInfo1, StartPos1, EndPos1,ECC_Visibility, Params1);

	if ( bHit1 ) {
		
		// 2. Collision Check - LineTrace 2nd
		// 1) From Muzzle
		FVector StartPos2=NormalGun->NormalGunMesh->GetSocketLocation( TEXT( "Muzzle" ) );
		// 2) To Collision Position 
		FVector EndPos2= HitInfo1.Location + Player->GetFollowCamera()->GetForwardVector() * 1;
		
		FCollisionQueryParams Params2;
		Params2.AddIgnoredActor( Player );
		GetWorld()->LineTraceSingleByChannel( HitInfo2, StartPos2, EndPos2, ECC_Visibility, Params2 );

		if (MainUI)
		{
			MainUI->SetCrossHairColor(HitInfo2.bBlockingHit && Cast<AEnemyBase>(HitInfo2.GetActor()));
		}
	}
	else
	{
		HitInfo2 = FHitResult();

		if (MainUI)
		{
			MainUI->SetCrossHairColor(false);
		}
	}
}

void UFireComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapePlayer>();
	check(Player);

	PC = Cast<AProjectEscapePlayerController>( GetWorld()->GetFirstPlayerController());
}


// Called every frame
void UFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bHasFired = false;
	CheckIfShootCanHit();
}

void UFireComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = PlayerInputComponent;
	PlayerInputComponent->BindAction(ActionFire, ETriggerEvent::Started, this, &UFireComponent::NormalGunFire);
	PlayerInputComponent->BindAction(ActionReload, ETriggerEvent::Started, this, &UFireComponent::BulletReload );
	PlayerInputComponent->BindAction(ActionAimDownSight, ETriggerEvent::Started, this, &UFireComponent::StartAimDown);
	PlayerInputComponent->BindAction(ActionAimDownSight, ETriggerEvent::Completed, this, &UFireComponent::EndAimDown);
}

void UFireComponent::SetGunVisibility(const bool ShowGun)
{
	if (NormalGun)
	{
		NormalGun->NormalGunMesh->SetVisibility(ShowGun);
	}
}

void UFireComponent::NormalGunFire()
{
	if (bHasPistol == false 
	|| Player->IsReloading == true
	|| Player->HasMatchingGameplayTag(PEGameplayTags::Status_IsDashing)) {
		return;
	}

	if (!Player->HasMatchingGameplayTag(PEGameplayTags::Status_CanShoot) && Player->GetMesh()->GetAnimInstance()->Montage_IsPlaying(FireMontage))
	{
		return;
	}

	if( Player->PlayerStatsComponent->CurrentBullets <= 0 )
	{
		BulletReload();
		return;
	}

	Player->RemoveGameplayTag(PEGameplayTags::Status_CanShoot);

	HandleFireAnimation();
	Player->PlayerStatsComponent->CurrentBullets--;

	if ( PC == nullptr ) {
		return;
	}
	PC->ClientStartCameraShake( GunShootCameraShakeEffect );
	
	PC->InGameWIdget->SetCurrentBullets();
		
	const float RecoilValue = FMath::RandRange(RecoilValueMin,RecoilValueMax);
	Player->AddControllerPitchInput(-RecoilValue);

	UGameplayStatics::PlaySound2D(GetWorld(), GunSoundClass);
	//FVector MuzzleLoc =NormalGun->NormalGunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );
	//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), MuzzleEffect, MuzzleLoc, FRotator(), FVector( 1 ), true, EPSCPoolMethod::None, true );
	UGameplayStatics::SpawnEmitterAttached( MuzzleEffect, NormalGun->NormalGunMesh ,FName(TEXT("Muzzle")),FVector::ForwardVector*1.0f, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,true);
	
	//FRotator GazeRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), Player->GetCameraBoom()->GetForwardVector() * MaxDistanceToGun);
	//Player->SetActorRotation(GazeRotation);

	if (MainUI)
	{
		MainUI->GunShot();
	}
	
	// =================== 데미지 체크 ===============

	if ( !HitInfo1.bBlockingHit )
	{
		return;
	}
	
	AEnemyBase* Enemy = nullptr;
	
	if( HitInfo2.bBlockingHit )
	{
		if(HitInfo2.GetActor()->IsA<AEnemyBase>() )
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), BloodEffect, HitInfo2.Location, HitInfo2.ImpactNormal.Rotation(), FireEffectScale * 3.f, true );
			

			Enemy=Cast<AEnemyBase>( HitInfo2.GetActor() );

			AActor* Actor=HitInfo2.GetActor();

			if ( Actor && Actor->GetRootComponent()->IsSimulatingPhysics() )
			{
				HitInfo2.Component->AddImpulse( HitInfo2.ImpactNormal * -1 * GunImpulseForce );

				if ( Cast<UStaticMeshComponent>( Actor->GetComponentByClass( UStaticMeshComponent::StaticClass() ) )->IsSimulatingPhysics() )
				{
					HitInfo2.Component->AddImpulse( HitInfo2.ImpactNormal * -1 * GunImpulseForce );
				}
			}
		}else
		{
			UE_LOG( LogTemp, Warning, TEXT( "hit actor: %s" ), *HitInfo2.GetActor()->GetActorNameOrLabel() )
			UDecalComponent* UdecalEffect = UGameplayStatics::SpawnDecalAtLocation( GetWorld(), WallDecalEffect, WallDecalScale, /*HitInfo2.GetComponent()->GetComponentLocation()*/ HitInfo2.ImpactPoint, HitInfo2.ImpactNormal.Rotation(), 10 );
			UdecalEffect->SetFadeScreenSize(0.f);
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), GunEffectNoActor, HitInfo2.TraceEnd, FRotator(), FireEffectScale, true );
			UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), GunEffectNoActor, HitInfo2.ImpactPoint, FRotator(), FireEffectScale, true );
		}
	}
	else
	{
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), GunEffectNoActor, HitInfo2.TraceEnd, FRotator(), FireEffectScale, true );
		AActor* Actor = HitInfo1.GetActor();
		if (Actor && Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()))
		{
			if (Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()))->IsSimulatingPhysics())
			{
				HitInfo2.Component->AddImpulse(HitInfo2.ImpactNormal * -1 * GunImpulseForce);
			}
		}
	}
	
	if (Enemy)
	{
		OnEnemyHitByPlayerGun.Broadcast(Enemy, HitInfo2);
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
	Player->GetCameraBoom()->TargetArmLength = 60;
	Player->GetCameraBoom()->SetRelativeLocation(AdsOffset);
	Player->GetCameraBoom()->bEnableCameraLag = false;
	Player->GetFollowCamera()->FieldOfView = 85.f;
}

void UFireComponent::EndAimDown(const FInputActionInstance& InputActionInstance)
{
	Player->GetCameraBoom()->TargetArmLength = 275;
	Player->GetCameraBoom()->SetRelativeLocation(FVector::ZeroVector);
	Player->GetCameraBoom()->bEnableCameraLag = true;
	Player->GetFollowCamera()->FieldOfView = 90.f;
}

void UFireComponent::BulletReload()
{
	//GameTag로 변경하기
	// 장전하고 있으면 장전 못함, 염력 사용중이면 장전 못함
	if (
			Player->IsReloading == true
		||	Player->GrabComponent->bIsGrabbing == true
		||	Player->GetCurrentMontage() == ReloadMontage
		||	Player->PlayerStatsComponent->CurrentBullets >= Player->PlayerStatsComponent->MaxBullets
		)
	{
		return;
	}

	PlayReloadAnimation();

	//auto AnimInst = Cast<UProjectEscapeAnimInstance>( Player->GetMesh()->GetAnimInstance() );
	//AnimInst->PlayReloadAnimation(); // 애니메이션 끝나고 InitBullets함수 실행

}

void UFireComponent::InitBullets()
{
	Player->PlayerStatsComponent->CurrentBullets = Player->PlayerStatsComponent->MaxBullets;
	PC->InGameWIdget->SetCurrentBullets();
	Player->IsReloading = false;
	Player->AddGameplayTag(PEGameplayTags::Status_CanShoot);
}
