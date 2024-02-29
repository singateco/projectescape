// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Shooting.h"

#include "Particles/ParticleSystem.h"
#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "Enemy/EnemyBullet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"

UBTTask_Shooting::UBTTask_Shooting()
{
	NodeName = TEXT( "Shooting" );

    static ConstructorHelpers::FClassFinder<AEnemyBullet> BulletClassFinder{ TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/BP_EnemyBullet.BP_EnemyBullet_C'" ) };

    if ( BulletClassFinder.Succeeded() )
    {
        EnemyBulletFactory = BulletClassFinder.Class;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundFinder( TEXT( "/Script/Engine.SoundCue'/Game/Resources/KDE/Sound/GunFire_Cue.GunFire_Cue'" ) );

    if ( ShootingSoundFinder.Succeeded() )
    {
        ShootingSound = ShootingSoundFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashFinder( TEXT( "/Script/Engine.ParticleSystem'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/VFX/P_MuzzleFlash_01.P_MuzzleFlash_01'" ) );

    if ( MuzzleFlashFinder.Succeeded() )
    {
        MuzzleFlash= MuzzleFlashFinder.Object;
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

        auto Player = UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 );
        FVector DirectionToPlayer = (Player->GetActorLocation() - MuzzleLocation).GetSafeNormal();
        FRotator RotationToPlayer = DirectionToPlayer.Rotation();

        float RandAccuracy = FMath::RandRange( 0, 9 );
        if ( RandAccuracy < Accuracy )
        {
            GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLocation, RotationToPlayer );
        }
        else
        {
            float X=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
            float Y=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );
            float Z=UKismetMathLibrary::RandomFloatInRange( Spread * -1, Spread );

            GetWorld()->SpawnActor<AEnemyBullet>( EnemyBulletFactory, MuzzleLocation, RotationToPlayer + FRotator( X, Y, Z ) );
        }

        UGameplayStatics::SpawnEmitterAttached( MuzzleFlash, Boss->GunMesh, FName( TEXT( "Muzzle" ) ), FVector::ZeroVector, FRotator::ZeroRotator, FVector( 1 ), EAttachLocation::SnapToTarget, true );
        UGameplayStatics::PlaySoundAtLocation( GetWorld(), ShootingSound, MuzzleLocation, FRotator() );

        FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
        return EBTNodeResult::Succeeded;
    }
    else
    {
        return EBTNodeResult::Failed;
    }
}
