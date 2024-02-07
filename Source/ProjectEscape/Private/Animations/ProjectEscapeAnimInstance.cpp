// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ProjectEscapeAnimInstance.h"

#include "CharacterBase.h"
#include "PECharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}
