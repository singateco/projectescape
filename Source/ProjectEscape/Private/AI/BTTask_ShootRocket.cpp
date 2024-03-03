// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ShootRocket.h"
#include "Particles/ParticleSystem.h"

#include "AI/BossAIController.h"
#include "Enemy/BossEnemy.h"
#include "Enemy/Rocket.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ShootRocket::UBTTask_ShootRocket()
{
	NodeName = TEXT( "ShootRocket" );

    static ConstructorHelpers::FClassFinder<ARocket> RocketFinder{ TEXT( "/Script/Engine.Blueprint'/Game/Blueprints/BP_Rocket.BP_Rocket_C'" ) };

    if ( RocketFinder.Succeeded() )
    {
        RocketFactory = RocketFinder.Class;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundFinder( TEXT( "/Script/Engine.SoundCue'/Game/Resources/KDE/Sound/Rocket_Cue.Rocket_Cue'" ) );

    if ( ShootingSoundFinder.Succeeded() )
    {
        ShootingSound=ShootingSoundFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleFlashFinder( TEXT( "/Script/Engine.ParticleSystem'/Game/Resources/KDE/GunsAndGrenade/Modern/Weapons/Assets/VFX/P_MuzzleFlash_01.P_MuzzleFlash_01'" ) );

    if ( MuzzleFlashFinder.Succeeded() )
    {
        MuzzleFlash = MuzzleFlashFinder.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ShootingFinder
    { TEXT( "/Script/Engine.AnimMontage'/Game/Animations/Actions/AM_MM_Pistol_Fire.AM_MM_Pistol_Fire'" ) };

    if ( ShootingFinder.Succeeded() )
    {
        ShootingMontage=ShootingFinder.Object;
    }

}


EBTNodeResult::Type UBTTask_ShootRocket::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABossAIController* Cont=Cast<ABossAIController>( OwnerComp.GetAIOwner() );
    APawn* Pawn=Cont->GetPawn();
    ABossEnemy* Boss=Cast<ABossEnemy>( Pawn );

    if(Boss )
    {
        FVector MuzzleLocation=Boss->GunMesh->GetSocketLocation( FName( TEXT( "Muzzle" ) ) );

        auto Player=UGameplayStatics::GetPlayerCharacter( GetWorld(), 0 );
        FVector DirectionToPlayer=(Player->GetActorLocation() - MuzzleLocation).GetSafeNormal();
        FRotator RotationToPlayer=DirectionToPlayer.Rotation();

        GetWorld()->SpawnActor<ARocket>( RocketFactory, MuzzleLocation, RotationToPlayer );
        UGameplayStatics::SpawnEmitterAttached( MuzzleFlash, Boss->GunMesh, FName( TEXT( "Muzzle" ) ), FVector::ZeroVector, FRotator::ZeroRotator, FVector( 1 ), EAttachLocation::SnapToTarget, true );
        UGameplayStatics::PlaySoundAtLocation( GetWorld(), ShootingSound, MuzzleLocation, FRotator() );

        UAnimInstance* AnimInstance=Boss->GetMesh()->GetAnimInstance();
        AnimInstance->Montage_Play( ShootingMontage );

        FinishLatentTask( OwnerComp, EBTNodeResult::Succeeded );
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
