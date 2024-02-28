// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyThrowActor.h"


AEnemyThrowActor::AEnemyThrowActor()
{

	GrenadeMesh->SetRelativeScale3D( FVector( 1.0f ) );
	GrenadeMesh->SetRelativeRotation( FRotator( 0, 0, 0 ) );
	GrenadeMesh->SetSimulatePhysics( false );
	ExplosionEffectSize = 0.5f;
	ExplosionTime = 10.0f;
}

void AEnemyThrowActor::OnMeshBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnMeshBeginHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	Explosion();

}
