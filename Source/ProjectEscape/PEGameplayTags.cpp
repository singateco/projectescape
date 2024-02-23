#include "PEGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace PEGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rarity_Normal, "Rarity.Normal", "일반")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rarity_Uncommon, "Rarity.Uncommon", "고급")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rarity_Rare, "Rarity.Rare", "희귀")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rarity_Epic, "Rarity.Epic", "영웅")
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Gun_Damage, "Stat.Gun.Damage", "총 기본 데미지")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Gun_DamageMultiplier, "Stat.Gun.DamageMultiplier", "총 데미지 배수")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Gun_WeakPointMultiplier, "Stat.Gun.WeakPointMultiplier", "약점 데미지 배수");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Gun_FireRate, "Stat.Gun.FireRate", "총 발사 속도");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Gun_ReloadSpeedRate, "Stat.Gun.ReloadSpeedRate", "총 장전 속도");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Grab_DamageValue, "Stat.Grab.DamageValue", "던지기 데미지");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stat_Grab_ExplosionRadius, "Stat.Grab.ExplosionRadius", "던지기 폭발 범위");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_IsDead, "Status.IsDead", "타겟이 죽어있다.")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_IsReload, "Status.IsReload", "장전중.")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_IsDashing, "Status.IsDashing", "대시를 하고 있다.")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_IsADS, "Status.IsADS", "조준사격을 하고 있다.")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_CanShoot, "Status.CanShoot", "사격을 할 수 있다.")
}