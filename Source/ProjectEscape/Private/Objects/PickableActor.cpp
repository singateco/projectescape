// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/PickableActor.h"

#include "Components/SphereComponent.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBaseFSM.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"

// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp=CreateDefaultSubobject<USphereComponent>( TEXT( "CollisionComp" ) );
	SetRootComponent( CollisionComp );

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( RootComponent );
	MeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireEffectFinder{ TEXT( "/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'" ) };
	if ( FireEffectFinder.Succeeded() )
	{
		GunEffect=FireEffectFinder.Object;
	}
	//Box->SetGenerateOverlapEvents( true );
	//Box->SetCollisionProfileName( TEXT( "GrabObject" ) );
}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentHit.AddDynamic(this, &APickableActor::OnCompHit );
}

// Called every frame
void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableActor::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	const TArray<AActor*> ActorsToIgnoreArray;
	TArray<AActor*> OutActorsArray;

	bool bSphereOverlapResult = UKismetSystemLibrary::SphereOverlapActors( GetWorld(), this->GetActorLocation(), SphereRadius, ObjectTypes, AEnemyBase::StaticClass(), ActorsToIgnoreArray, OutActorsArray);
	 
	if( bSphereOverlapResult )
	{

		for ( AActor* HitActor : OutActorsArray )
		{
			//auto Enemy=Cast<AEnemyBase>( HitActor );
			auto Enemy=Cast<ACharacterBase>( HitActor );
			if ( Enemy )
			{
				//Enemy->EnemyBaseFSM->OnTakeDamage();
				//Enemy.
			}

			UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), GunEffect, this->GetActorLocation(), FRotator() , FVector(10), true, EPSCPoolMethod::None, true);

			this->Destroy();
		}
		
	}
}
