﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */

struct FInputActionInstance;
class UInputAction;
class URadialSlider;
class UTextBlock;
class AProjectEscapePlayer;
class UProgressBar;
class UImage;

UCLASS()
class UMainUI : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeTick( const FGeometry& MyGeometry, float InDeltaTime ) override;
public:
	UPROPERTY(EditDefaultsOnly)
	AProjectEscapePlayer* Player;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* CrossHair;

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidgetAnim), Transient )
	class UWidgetAnimation* DamageUIAnim;

	UFUNCTION()
	void PlayDamageAnimation();

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UTextBlock* BulletText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	URadialSlider* BulletSlider;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UImage* IMG_QSkill;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UImage* IMG_ESkill;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UProgressBar* PRB_QSkill;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UProgressBar* PRB_ESkill;
	
	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UTextBlock* TXT_QSkill;

	UPROPERTY( BlueprintReadWrite, meta=(BindWidget) )
	UTextBlock* TXT_ESkill;

	UFUNCTION(BlueprintCallable)
	void SetCrossHairColor(const bool EnemySeen);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowEnemyHit(const bool IsDyingHit);

	UFUNCTION(BlueprintImplementableEvent)
	void GunShot();
	
	UFUNCTION()
	void SetCurrentBullets();

	UFUNCTION()
	void SetQSkillCoolTimer(int32 CurrentTime, int32 MaxTime);

	UFUNCTION()
	void SetESkillCoolTimer( int32 CurrentTime, int32 MaxTime );

	UFUNCTION()
	void EndQSkillUI();

	UFUNCTION()
	void EndESkillUI();

	UFUNCTION()
	void StartQSkillUI();

	UFUNCTION()
	void StartESkillUI();

	UFUNCTION(BlueprintImplementableEvent)
	void SkillKeyTriggered(const FInputActionInstance& ActionInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayQSkillCooldownAnim();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayESkillCooldownAnim();
	
protected:
	virtual void NativeOnInitialized() override;
};
