// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBullet.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ProjectEscapePlayer.h"
#include "ProjectEscape/PEGameplayTags.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(2.0f);
	SphereCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	SphereCollision->SetUseCCD(true);
	SphereCollision->SetNotifyRigidBodyCollision( true );

	EnemyBulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyBulletMesh"));
	EnemyBulletMesh->SetupAttachment(SphereCollision);
	EnemyBulletMesh->SetRelativeScale3D(FVector(1.0f, 0.03f, 0.03f));
	EnemyBulletMesh->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	ConstructorHelpers::FObjectFinder<UStaticMesh>BulletMesh(TEXT("/Script/Engine.StaticMesh'/Game/Material/TracerRound.TracerRound'"));

	if (BulletMesh.Succeeded())
	{
		EnemyBulletMesh->SetStaticMesh(BulletMesh.Object);
	}

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	MovementComponent->InitialSpeed = 10000;
	MovementComponent->MaxSpeed = 10000;
	MovementComponent->Bounciness = 0;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Friction = 0;
	MovementComponent->InterpLocationTime = 0.05f;
	MovementComponent->ProjectileGravityScale = 0;

	SetLifeSpan( 3.0f );
}



// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	if(SphereCollision)
	{
		SphereCollision->OnComponentHit.AddDynamic( this, &AEnemyBullet::OnSphereComponentBeginHit );
	}

}

void AEnemyBullet::OnSphereComponentBeginHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{

	if (AProjectEscapePlayer* Player = Cast<AProjectEscapePlayer>(OtherActor))
	{
		if (Cast<USkeletalMeshComponent>(OtherComp))
		{
			Player->ProcessDamageFromLoc(BulletDamage, Hit);
			this->Destroy();

			if (!Player->HasMatchingGameplayTag(PEGameplayTags::Status_CantBeDamaged))
			{
				UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(), BulletDecalBlood, FVector(10), Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10);
				Decal->SetFadeScreenSize(0.f);
				
				UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), BulletImpactBlood, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), FVector( 1 ), true, true, ENCPoolMethod::AutoRelease );
				UGameplayStatics::PlaySoundAtLocation( GetWorld(), BulletHitSoundBlood, Hit.ImpactPoint );
			}
			return;
		}
	}

	UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecalWall, FVector(10),Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), 10);
	Decal->SetFadeScreenSize(0.f);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), BulletImpactWall, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), FVector( 1 ), true, true, ENCPoolMethod::AutoRelease );
	UGameplayStatics::PlaySoundAtLocation( GetWorld(), BulletHitSoundWall, Hit.ImpactPoint );
	this->Destroy();
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


