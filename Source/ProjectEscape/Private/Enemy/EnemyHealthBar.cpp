// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float newPercent = FMath::Lerp(HPBarGuide->GetPercent(), TargetPercent, InDeltaTime * 5);
	HPBarGuide->SetPercent(newPercent);

}

void UEnemyHealthBar::UpdateHP(int HP, int MaxHP)
{
	TargetPercent = (float)HP / MaxHP;
	HPBar->SetPercent(TargetPercent);
	
	
}
