// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/GrenadeEnemyFSM.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/Grenade.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

UGrenadeEnemyFSM::UGrenadeEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AEnemyBullet> BulletClassFinder {TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_EnemyBullet.BP_EnemyBullet_C'")};
	if (BulletClassFinder.Succeeded())
	{
		EnemyBulletFactory = BulletClassFinder.Class;
	}
	
	AttackDistance = 3000;

	MinAttackTime = 5.0f;
	MaxAttackTime = 6.0f;

	Accuracy = 3;
	Spread = 5.0f;

}

// Called when the game starts
void UGrenadeEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrenadeEnemyFSM::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

void UGrenadeEnemyFSM::TickDamage()
{
	Super::TickDamage();
	bCanShoot = true;

}

void UGrenadeEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();
	ChangeGrenadeTime += GetWorld()->GetDeltaSeconds();
	AttackTime = FMath::RandRange( MinAttackTime, MaxAttackTime );

	FVector MuzzleLoc = Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

	if ( CurrentTime > AttackTime && bCanShoot == true)
	{
		CurrentTime = 0;
		//슛 몽타주가 문제있는듯
		//EnemyAnim->PlayShootMontage();
		FVector DirectionToPlayer = (Player->GetActorLocation() - MuzzleLoc).GetSafeNormal();
		FRotator RotationToPlayer = DirectionToPlayer.Rotation();
		check( EnemyBulletFactory );

		float RandAccuracy = FMath::RandRange( 0, 9 );
		if ( RandAccuracy < Accuracy )
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

	if(ChangeGrenadeTime > GrenadeTime )
	{
		Ai->StopMovement();
		bCanShoot = false;
		ChangeGrenadeTime = 0;
		EnemyAnim->PlayGrenadeMontage();
		Enemy->GunMesh->SetVisibility( false );
	}

	float dist=FVector::Dist( Player->GetActorLocation(), Enemy->GetActorLocation() );
	// 그 거리가 AttackDistance를 초과한다면
	if ( dist > AttackDistance || bCanSeePlayer == false ) {
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
	}

}

void UGrenadeEnemyFSM::ThrowGrenade()
{
	FVector DirectionToPlayer = (Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();

	FVector Impulse = DirectionToPlayer * GrenadeSpeed;
	Impulse.Z += AddVertical;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	EnemyGrenade = GetWorld()->SpawnActor<AGrenade>( EnemyGrenadeFactory, Enemy->GetMesh()->GetSocketTransform( FName( TEXT( "RightHandSocket" ) ) ), Params);

	if (EnemyGrenade)
	{
		EnemyGrenade->GrenadeMesh->SetPhysicsLinearVelocity( FVector::ZeroVector );
		EnemyGrenade->GrenadeMesh->AddImpulse(Impulse);
	}
}






