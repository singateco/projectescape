// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace PEGameplayTags
{
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Normal);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Uncommon);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Rare);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rarity_Epic);

	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Gun_Damage);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Gun_DamageMultiplier);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Gun_WeakPointMultiplier);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Gun_FireRate);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Gun_ReloadSpeedRate);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Grab_DamageValue);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stat_Grab_ExplosionRadius);
	
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsDead);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsReload);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsDashing);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsADS);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_CanShoot);
	PROJECTESCAPE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_CantBeDamaged);
}