// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RifleEnemyFSM.h"

#include "NiagaraFunctionLibrary.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/RifleEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

// Sets default values for this component's properties
URifleEnemyFSM::URifleEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AttackDistance = 3000;

	MinAttackTime = 0.5f;
	MaxAttackTime = 1.0f;

	Accuracy = 3;
	Spread = 5.0f;
}


// Called when the game starts
void URifleEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void URifleEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URifleEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();
	AttackTime = FMath::RandRange( MinAttackTime, MaxAttackTime );
	FVector MuzzleLoc = Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

	if(CurrentTime > AttackTime)
	{
		CurrentTime = 0;
		EnemyAnim->PlayShootMontage();

		FVector DirectionToPlayer = (Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		FRotator RotationToPlayer = DirectionToPlayer.Rotation();

		check( EnemyBulletFactory );

		float RandAccuracy = FMath::RandRange( 0, 9 );
		if(RandAccuracy < Accuracy )
		{
			GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLoc, RotationToPlayer );
		}
		else
		{
			float X=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
			float Y=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
			float Z=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );

			GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLoc, RotationToPlayer + FRotator( X, Y, Z ) );
		}

		UGameplayStatics::SpawnEmitterAttached( MuzzleFlash, Enemy->GunMesh, FName( TEXT( "Muzzle" ) ), FVector::ZeroVector, FRotator::ZeroRotator, FVector( 1 ), EAttachLocation::SnapToTarget, true );
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), AttackSound, MuzzleLoc );
	}

	float dist = FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation());
	// 그 거리가 AttackDistance를 초과한다면
	if (dist > AttackDistance || bCanSeePlayer == false) {
		// 이동상태로 전이하고싶다.
		SetState(EEnemyState::Move);
		//EnemyAnim->IsAttack = false;
	}
}


