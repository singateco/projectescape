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
	//GrenadeMesh->SetupAttachment( RootComponent );
	GrenadeMesh->SetRelativeScale3D( FVector( 2.0f ) );
	GrenadeMesh->SetSimulatePhysics( true );

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempGrenadeMesh( TEXT( "/Script/Engine.StaticMesh'/Game/Resources/KDE/FPS_Weapon_Bundle/Weapons/Meshes/G67_Grenade/SM_G67_Thrown.SM_G67_Thrown'" ) );

	if ( TempGrenadeMesh.Succeeded() )
	{
		GrenadeMesh->SetStaticMesh( TempGrenadeMesh.Object );
	}

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

	//DrawDebugSphere( GetWorld(), GrenadeMesh->GetComponentLocation(), SphereRadius, 100, FColor::Red, false, 3 );

	bool bSphereOverlapResult = UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GrenadeMesh->GetComponentLocation(), SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter = Cast<AProjectEscapePlayer>( HitActor );
			if ( OtherCharacter )
			{
				FVector StartLoc = GrenadeMesh->GetComponentLocation();
				FVector EndLoc = OtherCharacter->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor( this );
				Params.AddIgnoredActor( HitActor );

				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);
				// 라인 트레이스에서 플레이어가 충돌한 경우 데미지를 입힘
				if ( !bHit )
				{
					OtherCharacter->ProcessDamage( 10 );
				}
			}
		}

		UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ExplosionEffect, GrenadeMesh->GetComponentLocation(), FRotator(), FVector( 10 ), true, EPSCPoolMethod::None, true );

		this->Destroy();

	}
}

