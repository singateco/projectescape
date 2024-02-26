// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SniperEnemyFSM.h"

#include "AIController.h"
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

	AttackTime = 4.0f;

}

void USniperEnemyFSM::TickAttack()
{
	Ai->StopMovement();
	auto SniperEnemy=Cast<ASniperEnemy>( Enemy );

	FVector DirectionToPlayer=(Player->GetActorLocation() - SniperEnemy->LaserBeam->GetComponentLocation()).GetSafeNormal();
	FRotator RotationToPlayer = DirectionToPlayer.Rotation();

	//FRotator RotationToPlayer=UKismetMathLibrary::FindLookAtRotation( SniperEnemy->LaserBeam->GetComponentLocation(), Player->GetActorLocation() );
	SniperEnemy->LaserBeam->SetHiddenInGame( false );
	SniperEnemy->LaserBeam->SetWorldRotation( RotationToPlayer);

	CurrentTime += GetWorld()->GetDeltaSeconds();
	FVector MuzzleLoc = Enemy->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

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
		SniperEnemy->LaserBeam->SetHiddenInGame( true );
		SetState( EEnemyState::Move );
		
	}

}

void USniperEnemyFSM::TickDie()
{
	Super::TickDie();
	auto SniperEnemy=Cast<ASniperEnemy>( Enemy );
	SniperEnemy->LaserBeam->SetHiddenInGame( true );
}
