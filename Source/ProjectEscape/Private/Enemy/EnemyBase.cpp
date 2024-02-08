// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyAIPerception.h"
#include "Enemy/EnemyBaseFSM.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyBaseFSM = CreateDefaultSubobject<UEnemyBaseFSM>(TEXT("EnemyBaseFSM"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	EnemyHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHPComponent"));
	EnemyHPComponent->SetupAttachment(RootComponent);
	//WBP(블루프린트 클래스)를 로드해서 HPComp의 위젯으로 설정, FClassFinder 주소 마지막에 _C해야함 블루프린트라서
	ConstructorHelpers::FClassFinder<UUserWidget> tempHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C'"));

	if (tempHP.Succeeded())
	{
		EnemyHPComponent->SetWidgetClass(tempHP.Class);
		EnemyHPComponent->SetDrawSize(FVector2D(100, 20));
		EnemyHPComponent->SetRelativeLocation(FVector(0, 0, 90));
		EnemyHPComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

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
	FVector end = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	FRotator newRoatation = UKismetMathLibrary::FindLookAtRotation(start, end);

	EnemyHPComponent->SetWorldRotation(newRoatation);
}




