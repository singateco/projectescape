﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RifleEnemy.h"

ARifleEnemy::ARifleEnemy(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/Swat.Swat'"));

	if (TempEnemyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempEnemyMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Rifles/01_HQ/SKM_Modern_Weapons_Rifle_01_HQ.SKM_Modern_Weapons_Rifle_01_HQ'" ) );

	if ( TempGunMesh.Succeeded() )
	{
		GunMesh->SetSkeletalMesh( TempGunMesh.Object );
		//GunMesh->SetRelativeLocationAndRotation( FVector( 0, -7.3, 2.75 ), FRotator(6.6,-97 ,90));
		GunMesh->SetRelativeScale3D( FVector( 1.1f ) );
	}

	MaxHP = 6;

	//EnemyName = TEXT( "소총병" );
}



