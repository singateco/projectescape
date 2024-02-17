// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Grenade.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::Explosion()
{

	//UKismetSystemLibrary::SphereOverlapActors( GetWorld(), );


}

