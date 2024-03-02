// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Dash.h"

#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Dash::UBTTask_Dash()
{
	NodeName = TEXT( "Dash" );

	const static ConstructorHelpers::FObjectFinder<UAnimMontage> LeftDashFinder
	{ TEXT( "/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Dash_Left.AM_MM_Dash_Left'" ) };

	if ( LeftDashFinder.Succeeded() )
	{
		LeftDashMontage=LeftDashFinder.Object;
	}

	const static ConstructorHelpers::FObjectFinder<UAnimMontage> RightDashFinder
	{ TEXT( "/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Dash_Right.AM_MM_Dash_Right'" ) };

	if ( RightDashFinder.Succeeded() )
	{
		RightDashMontage=RightDashFinder.Object;
	}

	const static ConstructorHelpers::FObjectFinder<UAnimMontage> BackDashFinder
	{ TEXT( "/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Dash_Backward.AM_MM_Dash_Backward'" ) };

	if ( BackDashFinder.Succeeded() )
	{
		BackDashMontage=BackDashFinder.Object;
	}

}

void UBTTask_Dash::PlayAnimMontage()
{

}

EBTNodeResult::Type UBTTask_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABossAIController* Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
	APawn* Pawn=Cont->GetPawn();
	ABossEnemy* Boss=Cast<ABossEnemy>( Pawn );
	auto BossMove=Cast<UCharacterMovementComponent>( Boss );
	if(BossMove )
	{
		BossMove->AddImpulse( MoveVector * DashForce, true );
		PlayAnimMontage();

		FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;

}
