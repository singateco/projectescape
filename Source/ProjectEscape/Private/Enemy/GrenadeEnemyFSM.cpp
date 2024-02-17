// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/GrenadeEnemyFSM.h"

#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBullet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

UGrenadeEnemyFSM::UGrenadeEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

	AttackDistance = 3000;

	MinAttackTime = 0.5f;
	MaxAttackTime = 1.0f;

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

void UGrenadeEnemyFSM::TickAttack()
{
	CurrentTime+=GetWorld()->GetDeltaSeconds();
	AttackTime=FMath::RandRange( MinAttackTime, MaxAttackTime );

	if ( CurrentTime > AttackTime )
	{
		CurrentTime=0;
		EnemyAnim->PlayShootMontage();

		FVector DirectionToPlayer=(Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		FRotator RotationToPlayer=DirectionToPlayer.Rotation();

		check( EnemyBulletFactory );

		float RandAccuracy=FMath::RandRange( 0, 9 );
		if ( RandAccuracy < Accuracy )
		{
			GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) ), RotationToPlayer );
		}
		else
		{
			float X=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
			float Y=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
			float Z=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );

			GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) ), RotationToPlayer + FRotator( X, Y, Z ) );
		}

	}



	float dist=FVector::Dist( Player->GetActorLocation(), Enemy->GetActorLocation() );
	// 그 거리가 AttackDistance를 초과한다면
	if ( dist > AttackDistance || bCanSeePlayer == false ) {
		// 이동상태로 전이하고싶다.
		SetState( EEnemyState::Move );
		//EnemyAnim->IsAttack = false;
	}


}
