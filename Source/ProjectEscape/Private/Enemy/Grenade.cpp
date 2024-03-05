// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Grenade.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ProjectEscapePlayer.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "GrenadeMesh" ) );
	SetRootComponent( GrenadeMesh );
	//GrenadeMesh->SetupAttachment( RootComponent );
	GrenadeMesh->SetRelativeScale3D( FVector( 2.0f ) );
	GrenadeMesh->SetSimulatePhysics( true );
	GrenadeMesh->SetNotifyRigidBodyCollision( true );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempGrenadeMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Grenades/01/SM_Modern_Weapons_Grenade_01.SM_Modern_Weapons_Grenade_01'" ) );

	if ( TempGrenadeMesh.Succeeded() )
	{
		GrenadeMesh->SetStaticMesh( TempGrenadeMesh.Object );
	}

	GrenadeRange = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "GrenadeRange" ) );
	GrenadeRange->SetupAttachment( RootComponent );
	GrenadeRange->SetVisibility( false );
	GrenadeRange->SetRelativeScale3D( FVector(5.0f) );
	GrenadeRange->SetCollisionEnabled( ECollisionEnabled::NoCollision );


	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeFinder{ TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/Camera/BP_CSEnemyExplosion.BP_CSEnemyExplosion_C'" ) };
	if ( CameraShakeFinder.Succeeded() )
	{
		CameraShake = CameraShakeFinder.Class;
	}

	static const ConstructorHelpers::FObjectFinder<UMaterialInterface> GrenadeDecalFinder{ TEXT( "/Script/Engine.MaterialInstanceConstant'/Game/Resources/KDE/UWC_Bullet_Holes/Instances/Decals/Burnt/MI_Burnt_1.MI_Burnt_1'" ) };

	if ( GrenadeDecalFinder.Succeeded() )
	{
		GrenadeDecal=GrenadeDecalFinder.Object;
	}

	SetLifeSpan( 10.0f );
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer( Handle, FTimerDelegate::CreateUObject(this, &AGrenade::Explosion), ExplosionTime, false );

	if ( GrenadeMesh )
	{
		GrenadeMesh->OnComponentHit.AddDynamic( this, &AGrenade::OnMeshBeginHit );
	}
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Explosion()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnoreArray;
	ActorsToIgnoreArray.Add( this );
	TArray<AActor*> OutActorsArray;

	FVector GrenadeLoc = GrenadeMesh->GetComponentLocation();

	//DrawDebugSphere( GetWorld(), GrenadeMesh->GetComponentLocation(), SphereRadius, 50, FColor::Red, false, 3 );

	bool bSphereOverlapResult = UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GrenadeLoc, SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter = Cast<AProjectEscapePlayer>( HitActor );
			if ( OtherCharacter )
			{
				FVector StartLoc = GrenadeLoc;
				FVector EndLoc = OtherCharacter->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor( this );
				Params.AddIgnoredActor( HitActor );

				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);
				// 라인 트레이스에서 플레이어가 충돌한 경우 데미지를 입힘
				if ( !bHit )
				{
					OtherCharacter->ProcessDamageFromLoc( 5, HitResult );
				}
			}
		}
	}
	//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, GrenadeLoc, FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );
	/*FRotator DecalRotation= FRotator( -180, 0, 0 );
	UDecalComponent* UdecalEffect=UGameplayStatics::SpawnDecalAtLocation( GetWorld(), GrenadeDecal, FVector( 500 ), GrenadeLoc, DecalRotation, 10 );
	UdecalEffect->SetFadeScreenSize( 0.f );*/
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, GrenadeLoc, FRotator(), FVector(ExplosionEffectSize), true, true, ENCPoolMethod::AutoRelease);
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSound, GrenadeLoc );
	UGameplayStatics::PlayWorldCameraShake( GetWorld(), CameraShake, GrenadeLoc, 0, ShakeRadius );

	Destroy();

}

void AGrenade::Explosion(const FHitResult& Hit)
{

}

void AGrenade::OnMeshBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	GrenadeRange->SetVisibility( true );
}

