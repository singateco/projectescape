﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyThrowActor.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ProjectEscapePlayer.h"


AEnemyThrowActor::AEnemyThrowActor()
{
	GrenadeMesh->SetRelativeScale3D( FVector( 1.0f ) );
	GrenadeMesh->SetRelativeRotation( FRotator( 0, 0, 0 ) );
	GrenadeMesh->SetSimulatePhysics( false );
	ExplosionEffectSize = 0.5f;
	SphereRadius = 300.0f;
	ExplosionTime = 10.0f;

	static const ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalFinder{ TEXT( "/Script/Engine.MaterialInstanceConstant'/Game/Resources/KDE/UWC_Bullet_Holes/Instances/Decals/Cracks/MI_Crack_7.MI_Crack_7'" ) };

	if ( DecalFinder.Succeeded() )
	{
		GrenadeDecal=DecalFinder.Object;
	}
}

void AEnemyThrowActor::Explosion(const FHitResult& Hit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnoreArray;
	ActorsToIgnoreArray.Add( this );
	TArray<AActor*> OutActorsArray;

	FVector GrenadeLoc=GrenadeMesh->GetComponentLocation();

	bool bSphereOverlapResult=UKismetSystemLibrary::SphereOverlapActors( GetWorld(), GrenadeLoc, SphereRadius, ObjectTypes, nullptr, ActorsToIgnoreArray, OutActorsArray );

	if ( bSphereOverlapResult )
	{
		for ( AActor* HitActor : OutActorsArray )
		{
			auto OtherCharacter=Cast<AProjectEscapePlayer>( HitActor );
			if ( OtherCharacter )
			{
				FVector StartLoc=GrenadeLoc;
				FVector EndLoc=OtherCharacter->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor( this );
				Params.AddIgnoredActor( HitActor );

				bool bHit=GetWorld()->LineTraceSingleByChannel( HitResult, StartLoc, EndLoc, ECC_Visibility, Params );
				// 라인 트레이스에서 플레이어가 충돌한 경우 데미지를 입힘
				if ( !bHit )
				{
					OtherCharacter->ProcessDamageFromLoc( 5, HitResult );
				}
			}
		}
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), ExplosionEffect, GrenadeLoc, Hit.ImpactNormal.Rotation(), FVector( ExplosionEffectSize ), true );
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), ExplosionSound, GrenadeLoc );
	UGameplayStatics::PlayWorldCameraShake( GetWorld(), CameraShake, GrenadeLoc, 0, ShakeRadius );

	Destroy();

}

void AEnemyThrowActor::OnMeshBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnMeshBeginHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	Explosion(Hit);
	FRotator DecalRotation = Hit.ImpactNormal.Rotation() + FRotator( -180, 0, 0 );
	UDecalComponent* UdecalEffect=UGameplayStatics::SpawnDecalAtLocation( GetWorld(), GrenadeDecal, FVector( 300 ), Hit.ImpactPoint, DecalRotation, 10 );
	UdecalEffect->SetFadeScreenSize( 0.f );
}
