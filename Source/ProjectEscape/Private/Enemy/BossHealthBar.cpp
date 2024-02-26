// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossHealthBar.h"
#include "Components/ProgressBar.h"
#include "ProjectEscape/Public/CharacterBase.h"


void UBossHealthBar::NativeTick( const FGeometry& MyGeometry, float InDeltaTime )
{
	Super::NativeTick( MyGeometry, InDeltaTime );
	float newPercent=FMath::Lerp( HPBarGuide->GetPercent(), TargetPercent, InDeltaTime * 5 );
	HPBarGuide->SetPercent( newPercent );

}

void UBossHealthBar::UpdateHP( float MaxHP, float HP )
{
	TargetPercent=HP / MaxHP;
	HPBar->SetPercent( TargetPercent );
}

void UBossHealthBar::HPtoZero()
{
	HPBar->SetPercent( 0.f );
}

void UBossHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	if ( OwnedBoss )
	{
		OwnedBoss->GetStatsComponent()->OnHPChanged.AddUniqueDynamic( this, &UBossHealthBar::UpdateHP );
		OwnedBoss->GetStatsComponent()->OnHPReachedZero.AddUniqueDynamic( this, &UBossHealthBar::HPtoZero );
	}
}