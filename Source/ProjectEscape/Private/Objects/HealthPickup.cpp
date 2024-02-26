// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/HealthPickup.h"

#include "Components/SphereComponent.h"
#include "Player/PlayerStatsComponent.h"
#include "Player/ProjectEscapePlayer.h"


// Sets default values
AHealthPickup::AHealthPickup()
	:
	HitBox(CreateDefaultSubobject<USphereComponent>(TEXT("HitBox")))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(HitBox);
	
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

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	HitBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AHealthPickup::OnHitBoxOverlap);
}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

