// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Effect.h"

#include "Character/StatsComponent.h"

void UEffect::Initialize_Implementation()
{
	if (bIsUnique)
	{
		if (!OwningStatsComponent || !EffectOwner)
		{
			ConditionalBeginDestroy();
			return;
		}

		if (OwningStatsComponent->Effects.ContainsByPredicate([this](const UEffect* E)->bool
		{
			return E != this && E->Name == this->Name;
		}))
		{
			ConditionalBeginDestroy();
			return;
		}
	}

	
	switch (DurationType) {
	case EEffectDuration::Instant:
		Activate();
		EndEffect();
		break;
	case EEffectDuration::AfterDurationOnce:
		GetWorld()->GetTimerManager().SetTimer(
		AfterActivateDurationTimerHandle,
		FTimerDelegate::CreateLambda([&]
		{
			Activate();
			EndEffect();
		}),
		ActivateDurationSeconds,
		false);
		break;
	case EEffectDuration::AfterDuration:
		GetWorld()->GetTimerManager().SetTimer(
		AfterActivateDurationTimerHandle,
		this,
		&UEffect::Activate,
		ActivateDurationSeconds,
		false);
		break;
	case EEffectDuration::Constant:
		Activate();
		break;
	}	
}

void UEffect::Activate()
{
	OnEffectActivated.Broadcast(this);
	ActivateEffect();
}

void UEffect::EndEffect()
{
	if (AfterActivateDurationTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AfterActivateDurationTimerHandle);
		AfterActivateDurationTimerHandle.Invalidate();
	}
	OnEffectEnded.Broadcast(this);

	EffectEnded();
}
