// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ProjectEscapeAnimInstance.h"

#include "CharacterBase.h"
#include "PECharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/FireComponent.h"
#include "Player/MoveComponent.h"
#include "Player/ProjectEscapePlayer.h"

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

	const AProjectEscapePlayer* Player = Cast<AProjectEscapePlayer>(GetOwningActor());
	if (!Player)
	{
		return;
	}
	bHasFiring = Player->FireComponent->bHasFired;
	bIsDashing = Player->MoveComponent->bIsDashing;
}
