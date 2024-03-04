// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Rocket.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Player/ProjectEscapePlayer.h"

ARocket::ARocket()
{
	CollisionComp=CreateDefaultSubobject<USphereComponent>( TEXT( "CollisionComp" ) );
	SetRootComponent( CollisionComp );
	CollisionComp->SetSphereRadius( 10 );
	CollisionComp->SetCollisionProfileName( TEXT( "Projectile" ) );
	CollisionComp->SetNotifyRigidBodyCollision( true );

	RocketMesh=CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "RocketMesh" ) );
	RocketMesh->SetupAttachment( RootComponent );
	RocketMesh->SetRelativeScale3D( FVector( 3.0f ) );
	RocketMesh->SetRelativeRotation( FRotator( 0, -90, 0 ) );
	RocketMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempRocketMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Projectiles/SM_Modern_Weapons_Projectile_Launchers_02.SM_Modern_Weapons_Projectile_Launchers_02'" ) );

	if ( TempRocketMesh.Succeeded() )
	{
		RocketMesh->SetStaticMesh( TempRocketMesh.Object );
	}

	MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "MovementComponent" ) );

	MovementComponent->InitialSpeed = 1500;
	MovementComponent->MaxSpeed = 1500;
	MovementComponent->Bounciness = 0;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Friction = 0;
	MovementComponent->InterpLocationTime = 0.05f;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->bRotationFollowsVelocity = true;

	SetLifeSpan( 10.0f );

}

void ARocket::BeginPlay()
{
	Super::BeginPlay();

	if ( CollisionComp )
	{
		CollisionComp->OnComponentHit.AddDynamic( this, &ARocket::OnComponentBeginHit );
	}

}

void ARocket::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ARocket::Explosion()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnoreArray;
	ActorsToIgnoreArray.Add( this );
	TArray<AActor*> OutActorsArray;

	FVector RocketLoc = RocketMesh->GetComponentLocation();

	//DrawDebugSphere( GetWorld(), RocketMesh->GetComponentLocation(), SphereRadius, 50, FColor::Red, false, 3 );

	bool bSphereOverlapResult = UKismetSystemLibrary::SphereOverlapActors( GetWorld(), RocketLoc, SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter = Cast<AProjectEscapePlayer>( HitActor );
			if ( OtherCharacter )
			{
				FVector StartLoc = RocketLoc;
				FVector EndLoc = OtherCharacter->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor( this );
				Params.AddIgnoredActor( HitActor );

				bool bHit = GetWorld()->LineTraceSingleByChannel( HitResult, StartLoc, EndLoc, ECC_Visibility, Params );
				// 라인 트레이스에서 플레이어가 충돌한 경우 데미지를 입힘
				if ( !bHit )
				{
					OtherCharacter->ProcessDamageFromLoc(5, HitResult);
				}
			}
		}
	}

	//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, RocketLoc, FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, RocketLoc, FRotator(), FVector( ExplosionEffectSize ), true );
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSound, RocketLoc );

	this->Destroy();
}

void ARocket::OnComponentBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explosion();
}
