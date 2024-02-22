// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RocketEnemy.h"

ARocketEnemy::ARocketEnemy( const FObjectInitializer& ObjectInitializer )
	:
	Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/Ch35_nonPBR.Ch35_nonPBR'" ) );

	if ( TempEnemyMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh( TempEnemyMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0, 0, -90 ), FRotator( 0, -90, 0 ) );
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Launchers/02/SKM_Modern_Weapons_Launcher_02.SKM_Modern_Weapons_Launcher_02'" ) );

	if ( TempGunMesh.Succeeded() )
	{
		GunMesh->SetSkeletalMesh( TempGunMesh.Object );
		//GunMesh->SetRelativeLocationAndRotation( FVector( 0, -7.3, 2.75 ), FRotator( 6.6, -97, 90 ) );
		GunMesh->SetRelativeScale3D( FVector( 1.1f ) );
	}

	MaxHP = 4;
}

