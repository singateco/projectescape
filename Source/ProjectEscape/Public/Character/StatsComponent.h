// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatsComponent.generated.h"

class UEffect;
class ACharacterBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHPChanged, float, ChangedMaxHP, float, ChangedHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakenDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakenDamageFromLoc, const FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTagAdded, const FGameplayTag&, Tag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayTagRemoved, const FGameplayTag&, Tag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHPReachedZero);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayPriority = 1))
    float HP {3};

    UPROPERTY()
    float MaxHP {3};
	
public:
	// Sets default values for this component's properties
	UStatsComponent();

	UFUNCTION(BlueprintCallable)
	float GetHP() const { return HP; }

	UFUNCTION(BlueprintCallable)
	void SetHP(const float NewHP)
	{
		HP = NewHP;
		OnHPChanged.Broadcast(MaxHP, HP);
	}

	UFUNCTION(BlueprintCallable)
	float GetMaxHP() const { return MaxHP; }

	UFUNCTION(BlueprintCallable)
	void SetMaxHP(const float NewMaxHP)
	{
		MaxHP = NewMaxHP;

		if (HasBeenInitialized())
		{
			OnHPChanged.Broadcast(MaxHP, HP);
		}
	}

	/*
	 * 적용받고 있는 이펙트들.
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<UEffect*> Effects;

	UFUNCTION(BlueprintCallable)
	void AddEffect(UEffect* EffectToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveEffect(UEffect* EffectToRemove);

	UPROPERTY()
	ACharacterBase* OwningChara;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(BlueprintAssignable)
	FGameplayTagAdded OnGameplayTagAdded;

	UFUNCTION(BlueprintCallable)
	void AddTag(const FGameplayTag& TagToAdd);

	UPROPERTY(BlueprintAssignable)
	FGameplayTagRemoved OnGameplayTagRemoved;
	
	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& TagToRemove);

	UFUNCTION(BlueprintCallable)
	void ProcessDamage(const float DamageValue);

	UFUNCTION(BlueprintCallable)
	void ProcessDamageFromLoc(const float DamageValue, const FHitResult& HitResult);
	
	
	UFUNCTION(BlueprintCallable)
	void ProcessHealing(const float HealValue);

	UPROPERTY(BlueprintAssignable)
	FTakenDamage OnTakenDamage;

	UPROPERTY(BlueprintAssignable)
	FHPChanged OnHPChanged;
	
	void ProcessDying();

	UPROPERTY(BlueprintAssignable)
	FHPReachedZero OnHPReachedZero;

	UPROPERTY(BlueprintAssignable)
	FTakenDamageFromLoc OnTakenDamageFromLoc;
};
