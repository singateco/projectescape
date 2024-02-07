// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NormalGun.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"

// Sets default values
ANormalGun::ANormalGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create Normal Gun
	NormalGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NormalGun"));
	NormalGun->SetupAttachment(RootComponent);
	NormalGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto NormalGunMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));
	if (NormalGunMesh.Succeeded()) {
		NormalGun->SetStaticMesh(NormalGunMesh.Object);
		//SniperGun->SetRelativeLocation(FVector(0, 80, 130));
		NormalGun->SetRelativeScale3D(FVector(0.15f));
		NormalGun->SetRelativeLocationAndRotation(FVector(-38.f, -7.f, 3.f), FRotator(-2.f, 100.f, 0));
	}

}

// Called when the game starts or when spawned
void ANormalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

