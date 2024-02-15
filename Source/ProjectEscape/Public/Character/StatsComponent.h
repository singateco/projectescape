// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHPChanged, float, ChangedMaxHP, float, ChangedHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakenDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHPReachedZero);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HP {3};

    UPROPERTY()
    float MaxHP {3};
	
public:
	// Sets default values for this component's properties
	UStatsComponent();

	float GetHP() const { return HP; }
	void SetHP(const float NewHP)
	{
		HP = NewHP;
		OnHPChanged.Broadcast(MaxHP, HP);
	}

	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(const float NewMaxHP)
	{
		MaxHP = NewMaxHP;

		if (HasBeenInitialized())
		{
			OnHPChanged.Broadcast(MaxHP, HP);
		}
	}

	UPROPERTY()
	FGameplayTagContainer GameplayTagContainer;

	UFUNCTION(BlueprintCallable)
	void AddTag(const FGameplayTag& TagToAdd) { GameplayTagContainer.AddTag(TagToAdd); }

	UFUNCTION(BlueprintCallable)
	void RemoveTag(const FGameplayTag& TagToRemove) { GameplayTagContainer.RemoveTag(TagToRemove); }
	
	UFUNCTION()
	void ProcessDamage(const float DamageValue);

	UPROPERTY(BlueprintAssignable)
	FTakenDamage OnTakenDamage;

	UPROPERTY(BlueprintAssignable)
	FHPChanged OnHPChanged;
	
	FORCEINLINE void ProcessDying() const { OnHPReachedZero.Broadcast(); }

	UPROPERTY(BlueprintAssignable)
	FHPReachedZero OnHPReachedZero;
};
