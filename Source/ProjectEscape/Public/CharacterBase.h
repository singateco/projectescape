// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/StatsComponent.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class PROJECTESCAPE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned+
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Blueprintable, Category = "Stats")
	class UStatsComponent* StatsComponent;

	// -1이 아닐경우 MaxHP 설정
	UPROPERTY(EditAnywhere, Blueprintable, Category = "Stats")
	float MaxHP {-1};

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ProcessDamage(const float DamageValue) const { StatsComponent->ProcessDamage(DamageValue); } 
	
	UFUNCTION(BlueprintCallable)
	UStatsComponent* GetStatsComponent() const { return StatsComponent; }

	virtual void PreInitializeComponents() override;
};
