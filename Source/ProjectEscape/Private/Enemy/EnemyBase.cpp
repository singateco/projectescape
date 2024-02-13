﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyBaseFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"
#include "Perception/PawnSensingComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyBaseFSM = CreateDefaultSubobject<UEnemyBaseFSM>(TEXT("EnemyBaseFSM"));
	BulletREF = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletREF"));
	BulletREF->SetRelativeLocation(FVector(20, 0, 50));
	BulletREF->SetupAttachment(RootComponent);
	EnemyHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHPComponent"));
	EnemyHPComponent->SetupAttachment(RootComponent);
	//WBP(블루프린트 클래스)를 로드해서 HPComp의 위젯으로 설정, FClassFinder 주소 마지막에 _C해야함 블루프린트라서
	ConstructorHelpers::FClassFinder<UUserWidget> tempHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C'"));

	if (tempHP.Succeeded())
	{
		EnemyHPComponent->SetWidgetClass(tempHP.Class);
		EnemyHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
		EnemyHPComponent->SetDrawSize(FVector2D(80, 20));
		EnemyHPComponent->SetRelativeLocation(FVector(0, 0, 110));
		EnemyHPComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	UCapsuleComponent* cap = GetCapsuleComponent();
	auto mesh = GetMesh();

	cap->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	cap->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);



}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector start = EnemyHPComponent->GetComponentLocation();
	FVector end = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator newRoatation = UKismetMathLibrary::FindLookAtRotation(start, end);

	EnemyHPComponent->SetWorldRotation(newRoatation);
}

void AEnemyBase::OnSeePawn(APawn* Pawn)
{
	AProjectEscapePlayer* Player = Cast<AProjectEscapePlayer>(Pawn);
	if (Player)
	{
		bCanSeePlayer = true;
	}
	else
	{
		bCanSeePlayer = false;
	}
}

