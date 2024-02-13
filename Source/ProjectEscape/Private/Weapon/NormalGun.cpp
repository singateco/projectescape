// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/NormalGun.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"

// Sets default values
ANormalGun::ANormalGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	GunComp = CreateDefaultSubobject<USceneComponent>(TEXT("GunComp"));
	SetRootComponent(GunComp); // Socket Name

	// Create Normal Gun
	NormalGunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NormalGunMesh"));
	NormalGunMesh->SetupAttachment(GunComp);
	NormalGunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto NormalGunFinder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));
	if (NormalGunFinder.Succeeded()) {
		NormalGunMesh->SetStaticMesh(NormalGunFinder.Object);
		//SniperGun->SetRelativeLocation(FVector(0, 80, 130));
		NormalGunMesh->SetRelativeScale3D(FVector(0.15f));
		NormalGunMesh->SetRelativeLocationAndRotation(FVector(-38.f, -7.f, 3.f), FRotator(-2.f, 100.f, 0));
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

