﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/HealthPickup.h"

#include "FCTween.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"


// Sets default values
AHealthPickup::AHealthPickup()
	:
	HitBox(CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"))),
	MagnetBox(CreateDefaultSubobject<USphereComponent>(TEXT("Magnet Box")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(HitBox);
	MagnetBox->SetupAttachment(HitBox);
	
	SetLifeSpan(30.f);
}

void AHealthPickup::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProjectEscapePlayer* Player = Cast<AProjectEscapePlayer>(OtherActor);
	if (!Player)
	{
		return;
	}
	if (Player->PlayerStatsComponent->GetHP() >= Player->PlayerStatsComponent->GetMaxHP())
	{
		return;
	}

	Player->PlayerStatsComponent->ProcessHealing(HealValue);
	Destroy();
}

void AHealthPickup::OnMagnetBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AProjectEscapePlayer* Player = Cast<AProjectEscapePlayer>(OtherActor);
	if (!Player)
	{
		return;
	}
	if (Player->PlayerStatsComponent->GetHP() >= Player->PlayerStatsComponent->GetMaxHP())
	{
		return;
	}
	
	if (!MagnetTweenObj)
	{
		MagnetStartVector = GetActorLocation();
		PlayerTarget = Player;
		MagnetTweenObj = FCTween::Play(0.0f, 1.f,
							[&](const float v)
							{
								SetActorLocation(FMath::Lerp(MagnetStartVector, PlayerTarget->GetActorLocation(), v));
							},
							1.5f)
							->CreateUObject();
	}
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AHealthPickup::OnHitBoxOverlap);
	MagnetBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AHealthPickup::OnMagnetBoxOverlap);
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

