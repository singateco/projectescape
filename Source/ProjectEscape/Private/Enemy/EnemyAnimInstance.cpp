// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"

#include "Enemy/EnemyBase.h"
#include "Enemy/GrenadeEnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 오너를 가져와서 유효성 체크를 하고싶다.
	if ( nullptr == Enemy )
	{
		auto owner=TryGetPawnOwner();
		// 오너를 AEnemyBase로 캐스팅하고
		Enemy = Cast<AEnemyBase>( owner );
	}
	if ( nullptr == Enemy )
	{
		return;
	}
	// Actor Forward Vector
	FVector forward = Enemy->GetActorForwardVector();
	// Actor Right Vector
	FVector right = Enemy->GetActorRightVector();
	// Velocity
	FVector velocity = Enemy->GetVelocity();
	// Speed의 값을 채우고 싶다.
	Speed = FVector::DotProduct( forward, velocity );
	// Direction의 값을 채우고 싶다.
	Direction = FVector::DotProduct( right, velocity );
	// 캐릭터 무브먼트에서 IsFalling을 가져와서 IsFalling을 채우고싶다.
	UCharacterMovementComponent* movement = Enemy->GetCharacterMovement();
	IsFalling = movement->IsFalling();
}

void UEnemyAnimInstance::PlayShootMontage()
{
	if ( EnemyShootMontage )
	{
		Montage_Play( EnemyShootMontage );
	}
}

void UEnemyAnimInstance::PlayGrenadeMontage()
{
	if(EnemyGrenadeMontage )
	{
		Montage_Play( EnemyGrenadeMontage );
	}
}

void UEnemyAnimInstance::PlayHitAnimMontage()
{
	bHitMontage = FMath::RandBool();

	if(bHitMontage )
	{
		if ( EnemyHitMontage )
		{
			Montage_Play( EnemyHitMontage );
		}
	}
	else
	{
		if ( EnemyHitMontage2 )
		{
			Montage_Play( EnemyHitMontage2 );
		}
	}
	
}

void UEnemyAnimInstance::PlayDieAnimMontage()
{
	if(EnemyDieMontage)
	{
		Montage_Play( EnemyDieMontage );
	}
}

void UEnemyAnimInstance::AnimNotify_HitEnd()
{
	if(Enemy)
	{
		Enemy->EnemyBaseFSM->OnChangeMoveState();
		Montage_Stop( 0.1f, EnemyHitMontage);
	}
}

void UEnemyAnimInstance::AnimNotify_ThrowGrenade()
{
	if( Enemy )
	{
		auto FSM=Cast<UGrenadeEnemyFSM>( Enemy->EnemyBaseFSM );
		FSM->ThrowGrenade();
	}
	
}

void UEnemyAnimInstance::AnimNotify_FinishGrenade()
{
	if( Enemy )
	{
		auto FSM=Cast<UGrenadeEnemyFSM>( Enemy->EnemyBaseFSM );
		FSM->bCanShoot = true;
	}
}


