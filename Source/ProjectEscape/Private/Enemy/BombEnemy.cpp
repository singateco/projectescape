// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

ABombEnemy::ABombEnemy(const FObjectInitializer& ObjectInitializer)
	:
	Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick=true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempEnemyMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/Mesh/Dummy.Dummy'" ) );

	if ( TempEnemyMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh( TempEnemyMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation( FVector( 0, 0, -90 ), FRotator( 0, -90, 0 ) );
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh( TEXT( "/Script/Engine.SkeletalMesh'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/Explosives/01/SKM_Modern_Weapons_Explosive_01.SKM_Modern_Weapons_Explosive_01'" ) );

	if ( TempGunMesh.Succeeded() )
	{
		GunMesh->SetSkeletalMesh( TempGunMesh.Object );
		//GunMesh->SetRelativeLocationAndRotation( FVector( 0, -7.3, 2.75 ), FRotator(6.6,-97 ,90));
		GunMesh->SetRelativeScale3D( FVector( 2.0f ) );
	}

	MaxHP = 4;

	//EnemyName=TEXT( "자폭병" );

}

void ABombEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

}
