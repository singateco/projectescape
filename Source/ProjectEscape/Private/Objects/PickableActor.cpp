﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PickableActor.h"

#include "Components/SphereComponent.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBaseFSM.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstance.h"
#include "Player/GrabComponent.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "ProjectEscape/ProjectEscape.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"

#include "UI/MainUI.h"


// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CollisionComp=CreateDefaultSubobject<USphereComponent>( TEXT( "CollisionComp" ) );
	//SetRootComponent( CollisionComp );
	//CollisionComp->SetCollisionProfileName( TEXT( "PickUpActor" ) );
	//CollisionComp->SetCollisionEnabled( ECollisionEnabled::NoCollision ); 
	//CollisionComp->SetNotifyRigidBodyCollision( true ); // Simulation Generates Hit Events
	//CollisionComp->SetSimulatePhysics(true);
	//CollisionComp->SetSphereRadius(10);

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( RootComponent );
	MeshComp->SetCollisionProfileName( TEXT( "PickUpActor" ) );
	//MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision ); 
	MeshComp->SetNotifyRigidBodyCollision( true ); // Simulation Generates Hit Events
	MeshComp->SetSimulatePhysics( true );
	MeshComp->SetRelativeScale3D(FVector(1.6));


	//CollisionComp->SetRelativeScale3D( MeshComp->GetRelativeScale3D() );

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> FireEffectFinder{ TEXT( "/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'" ) };

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionObjEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/ExplosionsMegaPack/Niagara/Snow/NS_Explosion_Snow_3.NS_Explosion_Snow_3'" ) };

	if ( ExplosionObjEffectFinder.Succeeded() )
	{
		ExplosionObjEffect=ExplosionObjEffectFinder.Object;
	}


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/ExplosionsMegaPack/Niagara/Snow/NS_Explosion_Snow_1.NS_Explosion_Snow_1'" ) };

	if ( ExplosionEffectFinder.Succeeded() )
	{
		ExplosionEffect=ExplosionEffectFinder.Object;
	}

	//Box->SetCollisionProfileName( TEXT( "GrabObject" ) );


	// Blood VFX
	static const ConstructorHelpers::FObjectFinder<UNiagaraSystem> BloodEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Medium.OS_NS_Bullet_Hit_Medium'" ) };
	if ( BloodEffectFinder.Succeeded() )
	{
		BloodEffect=BloodEffectFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSoundFinder( TEXT( "/Script/Engine.SoundWave'/Game/Resources/KDE/Sound/S_LPAMG_Explosion_Grenade_01.S_LPAMG_Explosion_Grenade_01'" ) );

	if ( ExplosionSoundFinder.Succeeded() )
	{
		ExplosionSoundClass=ExplosionSoundFinder.Object;
	}



	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ExplosionCameraShakeEffectFinder( TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/Camera/BP_CSPlayerExplosion.BP_CSPlayerExplosion_C'" ) );

	if ( ExplosionCameraShakeEffectFinder.Succeeded() )
	{
		ExplosionCameraShakeEffect=ExplosionCameraShakeEffectFinder.Class;
	}





	static const ConstructorHelpers::FObjectFinder<UMaterialInterface> ExploDecalEffectFinder{ TEXT( "/Script/Engine.MaterialInstanceConstant'/Game/Resources/KDE/UWC_Bullet_Holes/Instances/Decals/Cracks/MI_Crack_7.MI_Crack_7'" ) };

	if ( ExploDecalEffectFinder.Succeeded() )
	{
		ExploDecalEffect=ExploDecalEffectFinder.Object;
	}


}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	//CollisionComp->OnComponentHit.AddDynamic( this, &APickableActor::OnCompHit );
	MeshComp->OnComponentHit.AddDynamic(this, &APickableActor::OnCompHit );
	Player=Cast<AProjectEscapePlayer>( GetWorld()->GetFirstPlayerController()->GetPawn() );
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickableActor::GetExplosionRadius()
{
	if (Player)
	{
		SphereRadius = Player->PlayerStatsComponent->GrabExplosionRadius;
		EmitterScaleValue = SphereRadius / 10.f;
	}
}

void APickableActor::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("%.1f"), NormalImpulse.Length());

	//if( this->MeshComp->GetComponentVelocity().Length() < 2000 )
	// {
	//	UE_LOG( SYLog, Warning, TEXT( "%.1f" ), this->MeshComp->GetComponentVelocity().Length() );
	//	 return;
	// }

	//if( this->MeshComp->GetComponentVelocity().Length() > 10000) // 던질때만 체크
	//if( this->MeshComp->GetComponentVelocity().Length() > 9000) // 던질때만 체크
	if (!Player || !Player->GrabComponent) return;
	if ( !bIsGrabbedObject ) return;
	if(Player->GrabComponent->bIsPushing == true )
	{
		
		//UE_LOG( SYLog, Warning, TEXT( "%.1f" ), this->MeshComp->GetComponentVelocity().Length() );
		//txt1=(Player->GrabComponent->bIsGrabbing) ? "true" : "false";
		//UE_LOG( SYLog, Warning, TEXT( "5000이상 bIsGrabbing : %s" ), *txt1 );

		//UE_LOG( SYLog, Warning, TEXT( "hitResult : %s" ), *Hit.GetActor()->GetActorNameOrLabel() );

		//UE_LOG( SYLog, Warning, TEXT( "hitComp : %s, ProfileCollision: %s" ), *OtherComp->GetFullName(), *OtherComp->GetCollisionProfileName().ToString());
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSoundClass, Hit.ImpactPoint, Hit.ImpactNormal.Rotation() );

		UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );

		FRotator DecalRotation = Hit.ImpactNormal.Rotation() + FRotator( -180, 0, 0 );

		UDecalComponent* UdecalEffect = UGameplayStatics::SpawnDecalAtLocation( GetWorld(), ExploDecalEffect, FVector(300), Hit.ImpactPoint, DecalRotation, 10);
		UdecalEffect->SetFadeScreenSize( 0.f );
		

		if( ExplosionCameraShakeEffect )
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake( ExplosionCameraShakeEffect );
		}

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		//ObjectTypes.Add(EObjectTypeQuery::)
		TArray<AActor*> ActorsToIgnoreArray;
		ActorsToIgnoreArray.Add( this );
		TArray<AActor*> OutActorsArray;

		GetExplosionRadius();

		//txt1=(Player->GrabComponent->bIsGrabbing) ? "true" : "false";
		//txt2=(Player->GrabComponent->bIsPulling) ? "true" : "false";
		//txt3=(Player->GrabComponent->bIsPushing) ? "true" : "false";
		//UE_LOG( SYLog, Warning, TEXT( "bIsGrabbing:%s, bIsPulling : %s, bIsPushing : %s" ), *txt1, *txt2, *txt3 );
		if(Hit.GetActor()->IsA<AEnemyBase>() ){
			auto OtherCharacter=Cast<AEnemyBase>( Hit.GetActor() );
			OtherCharacter->ProcessDamage( Player->PlayerStatsComponent->GrabDamageValue );
			
			const bool IsDyingHit = OtherCharacter->GetStatsComponent()->GetHP() <= 0;

			if (Player && Player->MainUI)
			{
				Player->MainUI->ShowEnemyHit(IsDyingHit);
			}
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease  );
			UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), BloodEffect, OtherCharacter->GetActorLocation(), FRotator(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );
			//GetActorLocation() 하면 안됨
			//UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSound, EnemyLoc );
		}

		bool bSphereOverlapResult=UKismetSystemLibrary::SphereOverlapActors( GetWorld(), this->MeshComp->GetComponentLocation(), SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );
		//bool bSphereOverlapResult=UKismetSystemLibrary::SphereOverlapActors( GetWorld(), this->GetActorLocation(), SphereRadius, ObjectTypes, AActor::StaticClass(), ActorsToIgnoreArray, OutActorsArray );

		if ( bSphereOverlapResult )
		{
			//UE_LOG( SYLog, Warning, TEXT( "hit!" ) );
			for ( AActor* HitActor : OutActorsArray )
			{
				//UE_LOG( SYLog, Warning, TEXT( "SphereOverlapResult:%s" ), *HitActor->GetActorNameOrLabel() );
				auto OtherCharacter=Cast<AEnemyBase>( HitActor );
				if ( OtherCharacter )
				{
					OtherCharacter->ProcessDamage( Player->PlayerStatsComponent->GrabDamageValue );

					UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), BloodEffect, OtherCharacter->GetActorLocation(), FRotator(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );

				}
				else
				{

					//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, this->MeshComp->GetComponentLocation(), FRotator(), FVector( EmitterScaleValue ), true, EPSCPoolMethod::None, true );

					UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );
				}

				//UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, HitActor->GetActorLocation(), FRotator(), ExplosionScale, true );
				UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );
			//UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionObjEffect, this->MeshComp->GetComponentLocation(), FRotator(), ExplosionScale, true );
		}

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionObjEffect, this->MeshComp->GetComponentLocation(), FRotator(), ExplosionScale, true, true, ENCPoolMethod::AutoRelease );
		//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, this->MeshComp->GetComponentLocation(), FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );
		this->Destroy();
		Player->GrabComponent->bIsPushing=false;
	}
	
}
