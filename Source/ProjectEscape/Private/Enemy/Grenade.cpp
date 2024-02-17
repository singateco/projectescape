// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Grenade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ProjectEscapePlayer.h"
#include "ProjectEscape/ProjectEscape.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "GrenadeMesh" ) );
	SetRootComponent( GrenadeMesh );
	GrenadeMesh->SetupAttachment( RootComponent );
	GrenadeMesh->SetRelativeScale3D( FVector( 2.0f ) );
	GrenadeMesh->SetSimulatePhysics( true );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempGrenadeMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Thrown.SM_G67_Thrown'" ) );

	if ( TempGrenadeMesh.Succeeded() )
	{
		GrenadeMesh->SetStaticMesh( TempGrenadeMesh.Object );
	}

	MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "MovementComponent" ) );

	MovementComponent->InitialSpeed = 2000;
	MovementComponent->MaxSpeed = 2000;
	//MovementComponent->Bounciness = 0.6;
	MovementComponent->bShouldBounce = true;
	//MovementComponent->Friction = 0.2 ;
	//MovementComponent->InterpLocationTime = 0.05f;
	MovementComponent->ProjectileGravityScale = 1;

	SetLifeSpan( 10.0f );
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer( Handle, FTimerDelegate::CreateLambda( [&]
		{
			Explosion();
		} ), ExplosionTime, false );

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

	DrawDebugSphere( GetWorld(), GrenadeMesh->GetComponentLocation(), SphereRadius, 100, FColor::Red, false, 3 );

	bool bSphereOverlapResult = UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GrenadeMesh->GetComponentLocation(), SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter=Cast<AProjectEscapePlayer>( HitActor );
			if ( OtherCharacter )
			{
				OtherCharacter->ProcessDamage(10);
			}
		}

		UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, GrenadeMesh->GetComponentLocation(), FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );
		

		this->Destroy();

	}
}

