// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ProjectEscapeAnimInstance.h"

#include "CharacterBase.h"
#include "PECharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/FireComponent.h"
#include "Player/ProjectEscapePlayer.h"
#include "ProjectEscape/PEGameplayTags.h"

void UProjectEscapeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ACharacterBase* Character = Cast<ACharacterBase>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UPECharacterMovementComponent* CharMoveComp = CastChecked<UPECharacterMovementComponent>(Character->GetMovementComponent());
	const FPECharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
	
	if (!Player)
	{
		return;
	}
	bIsDashing = Player->HasMatchingGameplayTag(PEGameplayTags::Status_IsDashing);
	bHasFiring = Player->FireComponent->bHasFired;
}



//void UProjectEscapeAnimInstance::PlayReloadAnimation()
//{
//	if ( ReloadMontage ) {
//		Montage_Play( ReloadMontage );
//	}
//}

void UProjectEscapeAnimInstance::AnimNotify_AN_Reload_C()
{
	//애니메이션이 끝난 후 함수 호출해주기 위해( 동작이 끝나고 재장전이 돼야한다.)
	if (Player)
	{
		Player->FireComponent->InitBullets();
	}
}

void UProjectEscapeAnimInstance::AnimNotify_SaveAttack()
{
	if (Player)
	{
		Player->AddGameplayTag(PEGameplayTags::Status_CanShoot);
	}
}

void UProjectEscapeAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Player = Cast<AProjectEscapePlayer>(GetOwningActor());
}
