// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Shooting.h"

#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "Enemy/EnemyBullet.h"
#include "Player/ProjectEscapePlayer.h"

UBTTask_Shooting::UBTTask_Shooting()
{
	NodeName = TEXT( "Shooting" );

    static ConstructorHelpers::FClassFinder<AEnemyBullet> BulletClassFinder{ TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/BP_EnemyBullet.BP_EnemyBullet_C'" ) };

    if ( BulletClassFinder.Succeeded() )
    {
        EnemyBulletFactory=BulletClassFinder.Class;
    }
}

EBTNodeResult::Type UBTTask_Shooting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABossAIController* Cont = Cast<ABossAIController>( OwnerComp.GetAIOwner() );
    APawn* Pawn = Cont->GetPawn();
    ABossEnemy* Boss = Cast<ABossEnemy>( Pawn );

    if ( Boss )
    {
        // 현재 캐릭터의 위치와 회전을 얻어옴
        FVector MuzzleLocation = Boss->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

        auto Player = Cast<AProjectEscapePlayer>( GetWorld()->GetFirstPlayerController()->GetPawn() );
        FVector DirectionToPlayer = (Player->GetActorLocation() - Boss->GetActorLocation()).GetSafeNormal();
        FRotator MuzzleRotation = DirectionToPlayer.Rotation();

        GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLocation, MuzzleRotation );

    }

    return EBTNodeResult::Succeeded;
}
