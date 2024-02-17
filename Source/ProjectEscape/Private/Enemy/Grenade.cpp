// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Grenade.h"
#include "Components/SphereComponent.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereCompnent" ) );
	SetRootComponent( SphereComponent );
	SphereComponent->SetSphereRadius( 10 );
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "GrenadeMesh" ) );
	GrenadeMesh->SetupAttachment( RootComponent );
	GrenadeMesh->SetRelativeScale3D( FVector( 2.0f ) );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempGrenadeMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Thrown.SM_G67_Thrown'" ) );

	if ( TempGrenadeMesh.Succeeded() )
	{
		GrenadeMesh->SetStaticMesh( TempGrenadeMesh.Object );
	}

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

