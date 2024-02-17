// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/GrenadeEnemy.h"

AGrenadeEnemy::AGrenadeEnemy(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/Ch35_nonPBR.Ch35_nonPBR'" ) );

	if ( TempEnemyMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh( TempEnemyMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0, 0, -90 ), FRotator( 0, -90, 0 ) );
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4_X.SK_AR4_X'" ) );

	if ( TempGunMesh.Succeeded() )
	{
		GunMesh->SetSkeletalMesh( TempGunMesh.Object );
		GunMesh->SetRelativeLocationAndRotation( FVector( 0, -7, 4 ), FRotator(14.5, -92, 90.5));
		GunMesh->SetRelativeScale3D( FVector( 1.1f ) );
	}

	MaxHP = 4;

}
