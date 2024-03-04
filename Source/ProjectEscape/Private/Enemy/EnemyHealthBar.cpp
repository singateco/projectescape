// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyHealthBar.h"
#include "Components/ProgressBar.h"
#include "Enemy/EnemyBase.h"

void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	float newPercent = FMath::Lerp(HPBarGuide->GetPercent(), TargetPercent, InDeltaTime * 5);
	HPBarGuide->SetPercent(newPercent);

}

void UEnemyHealthBar::UpdateHP(float MaxHP, float HP)
{
	TargetPercent = HP / MaxHP;
	HPBar->SetPercent(TargetPercent);
}

void UEnemyHealthBar::HPtoZero()
{
	HPBar->SetPercent(0.f);
}

//void UEnemyHealthBar::SetName()
//{
//	FString NormalEnemyName = OwnedEnemy->GetEnemyName();
//	EnemyName->SetText( FText::FromString( NormalEnemyName ) );
//}

void UEnemyHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwnedEnemy)
	{
		OwnedEnemy->GetStatsComponent()->OnHPChanged.AddUniqueDynamic(this, &UEnemyHealthBar::UpdateHP);
		OwnedEnemy->GetStatsComponent()->OnHPReachedZero.AddUniqueDynamic(this, &UEnemyHealthBar::HPtoZero);
	}
}
