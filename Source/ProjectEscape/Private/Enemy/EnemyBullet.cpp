// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(4.0f);
	SphereCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	EnemyBulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyBulletMesh"));
	EnemyBulletMesh->SetupAttachment(SphereCollision);
	EnemyBulletMesh->SetRelativeScale3D(FVector(0.1f));

	ConstructorHelpers::FObjectFinder<UStaticMesh>BulletMesh(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Ball_01.SM_Ball_01'"));

	if (BulletMesh.Succeeded())
	{
		EnemyBulletMesh->SetStaticMesh(BulletMesh.Object);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	MovementComponent->InitialSpeed = 3000;
	MovementComponent->MaxSpeed = 3000;
	MovementComponent->Bounciness = 0;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Friction = 0;
	MovementComponent->InterpLocationTime = 0.05f;
	MovementComponent->ProjectileGravityScale = 0;


}



// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	if(SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic( this, &AEnemyBullet::OnSphereComponentBeginHit );
	}
}

void AEnemyBullet::OnSphereComponentBeginHit( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	Destroy();
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


