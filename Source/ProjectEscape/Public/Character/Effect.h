// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

UENUM(BlueprintType)
enum class EEffectDuration : uint8
{
	Instant UMETA(DisplayName = "Instant", Tooltip = "1회 바로 적용후 삭제"),
	AfterDurationOnce UMETA(DisplayName = "AfterDurationOnce", Tooltip = "일정 시간 이후 1회 적용후 삭제"),
	AfterDuration UMETA(DisplayName = "AfterDuration", Tooltip = "일정 시간 이후 1회 적용후 남아있음"),
	Constant UMETA(DisplayName = "Constant", Tooltip = "시간 상관없이 계속 적용")
};


#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Effect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectActivated, UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectEnded, UEffect*, Effect);

class UStatsComponent;
class ACharacterBase;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTESCAPE_API UEffect : public UObject
{
	GENERATED_BODY()

public:

	// ============ 기본 설정 ===============
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	// 이미 같은 이름의 이펙트가 있다면 두번 적용되지 않음
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnique {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bIsUnique"))
	bool bCanStack {false};

	// 언제 발동될 것인지 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectDuration DurationType;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle AfterActivateDurationTimerHandle;

	// Duration 설정이 AfterDuration일 경우 이 시간 이후에 발동됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActivateDurationSeconds {1.0f};

	// =============== 게임 플레이 태그 =============
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer TagsToApply;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ConditionalTags;

	// =============== 기본 정보 =============
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacterBase* EffectOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStatsComponent* OwningStatsComponent;


	// ========== 함수 및 Delegates ===========
	UFUNCTION(BlueprintNativeEvent)
	void Initialize();
	
	UFUNCTION()
	void Activate();

	UFUNCTION(BlueprintNativeEvent)
	bool CheckCondition();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateEffect();

	UPROPERTY(BlueprintAssignable)
	FEffectActivated OnEffectActivated;
	
	UFUNCTION()
	void EndEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void EffectEnded();

	UPROPERTY(BlueprintAssignable)
	FEffectEnded OnEffectEnded;
};
