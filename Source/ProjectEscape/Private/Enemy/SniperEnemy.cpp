// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SniperEnemy.h"

ASniperEnemy::ASniperEnemy(const FObjectInitializer& ObjectInitializer)
	:
	Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick=true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/SWATGuy.SWATGuy'" ) );

	if ( TempEnemyMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh( TempEnemyMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0, 0, -90 ), FRotator( 0, -90, 0 ) );
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Snipers/03/SKM_Modern_Weapons_Sniper_03.SKM_Modern_Weapons_Sniper_03'" ) );

	if ( TempGunMesh.Succeeded() )
	{
		GunMesh->SetSkeletalMesh( TempGunMesh.Object );
		//GunMesh->SetRelativeLocationAndRotation( FVector( 0, -7.3, 2.75 ), FRotator(6.6,-97 ,90));
		GunMesh->SetRelativeScale3D( FVector( 1.1f ) );
	}

	LaserBeam = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "LaserBeam" ) );
	LaserBeam->SetupAttachment( GunMesh, TEXT("Laser") );
	LaserBeam->SetRelativeScale3D( FVector( 100.0f, 1.0f, 1.0f ) );
	LaserBeam->SetRelativeLocationAndRotation( FVector( 0, 0, 0 ), FRotator( 90, 0, 0 ) );
	LaserBeam->SetCollisionEnabled( ECollisionEnabled::NoCollision );
	LaserBeam->SetHiddenInGame( true );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempLaserMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/Effects/Laser/Laser_Beam.Laser_Beam'" ) );

	if ( TempLaserMesh.Succeeded() )
	{
		LaserBeam->SetStaticMesh( TempLaserMesh.Object );
	}

	MaxHP = 8;
}
