// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SniperEnemyFSM.h"

#include "AIController.h"
#include "Components/SplineMeshComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/SniperEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

USniperEnemyFSM::USniperEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;
	AttackDistance = 4000;
	AttackTime = 3.0f;
}

void USniperEnemyFSM::TickAttack()
{
	Ai->StopMovement();
	auto SniperEnemy=Cast<ASniperEnemy>( Enemy );

	FVector DirectionToPlayer;
	if (Player)
	{
		DirectionToPlayer = (Player->GetMesh()->GetBoneLocation(TEXT("Head")) - SniperEnemy->SplineLaserBeam->GetComponentLocation());
	}

	FRotator RotationToPlayer = DirectionToPlayer.GetSafeNormal().Rotation();

	//FRotator RotationToPlayer=UKismetMathLibrary::FindLookAtRotation( SniperEnemy->LaserBeam->GetComponentLocation(), Player->GetActorLocation() );

	CurrentTime += GetWorld()->GetDeltaSeconds();
	FVector MuzzleLoc = Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

	SniperEnemy->SplineLaserBeam->SetHiddenInGame( false );
	
	if (SniperEnemy->SplineLaserBeam)
	{
		SniperEnemy->SplineLaserBeam->SetEndPosition(DirectionToPlayer);
	}

	if ( CurrentTime > AttackTime )
	{
		CurrentTime = 0;
		EnemyAnim->PlayShootMontage();
		check( EnemyBulletFactory );

		GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLoc, RotationToPlayer );

		UGameplayStatics::SpawnEmitterAttached( MuzzleFlash, Enemy->GunMesh, FName( TEXT( "Muzzle" ) ), FVector::ZeroVector, FRotator::ZeroRotator, FVector( 1 ), EAttachLocation::SnapToTarget, true );
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), AttackSound, MuzzleLoc );
	}

	float dist = FVector::Dist( Player->GetActorLocation(), Enemy->GetActorLocation() );
	// 그 거리가 AttackDistance를 초과한다면
	if ( dist > AttackDistance || bCanSeePlayer == false ) {
		// 이동상태로 전이하고싶다.
		SniperEnemy->SplineLaserBeam->SetHiddenInGame( true );
		SetState( EEnemyState::Move );
	}
}

void USniperEnemyFSM::OnTakeDamage(float Damage_Unused)
{
	
}

void USniperEnemyFSM::TickDie()
{
	Super::TickDie();
	auto SniperEnemy=Cast<ASniperEnemy>( Enemy );
	SniperEnemy->SplineLaserBeam->SetHiddenInGame( true );
}
