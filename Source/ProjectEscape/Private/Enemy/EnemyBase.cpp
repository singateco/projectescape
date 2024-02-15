// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "NavigationInvokerComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyBaseFSM.h"

#include "Enemy/EnemyHealthBar.h"
#include "Enemy/EnemyStatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ProjectEscapePlayer.h"
#include "UI/DamageNumber.h"


AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer)
	:
	Super(ObjectInitializer.SetDefaultSubobjectClass<UEnemyStatsComponent>(TEXT("Stats Component")))
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyStatsComponent = Cast<UEnemyStatsComponent>(StatsComponent);
	EnemyBaseFSM = CreateDefaultSubobject<UEnemyBaseFSM>(TEXT("EnemyBaseFSM"));
	NavComponent=CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavComponent"));

	BulletREF = CreateDefaultSubobject<UArrowComponent>(TEXT("BulletREF"));
	BulletREF->SetRelativeLocation(FVector(40, 0, 50));
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

	ConstructorHelpers::FClassFinder<UUserWidget> DamageNumberWidgetFinder {TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_DamageNumber.WBP_DamageNumber_C'")};

	if (DamageNumberWidgetFinder.Succeeded())
	{
		DamageNumberWidgetClass = DamageNumberWidgetFinder.Class;
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	UCapsuleComponent* cap = GetCapsuleComponent();
	auto mesh = GetMesh();

	cap->SetCollisionProfileName( FName( "Enemy") );
	mesh->SetCollisionProfileName( FName( "NoCollision" ) );

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	check(DamageNumberWidgetClass);
	StatsComponent->OnTakenDamage.AddUniqueDynamic(this, &AEnemyBase::DisplayDamageNumber);
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector start = EnemyHPComponent->GetComponentLocation();
	FVector end = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FRotator newRoatation = UKismetMathLibrary::FindLookAtRotation(start, end);

	EnemyHPComponent->SetWorldRotation(newRoatation);
}

void AEnemyBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (EnemyHPComponent)
	{
		TSubclassOf<UUserWidget> WidgetClass = EnemyHPComponent->GetWidgetClass();
		EnemyHealthBarWidget = Cast<UEnemyHealthBar>(CreateWidget(GetWorld(), WidgetClass));
		EnemyHealthBarWidget->OwnedEnemy = this;
		EnemyHPComponent->SetWidget(EnemyHealthBarWidget);
	}
}

void AEnemyBase::DisplayDamageNumber(const float DamageToDisplay)
{
	UDamageNumber* DamageWidget = Cast<UDamageNumber>(CreateWidget(GetWorld(), DamageNumberWidgetClass));
	DamageWidget->Actor = this;
	DamageWidget->DamageToDisplay = DamageToDisplay;
	DamageWidget->AddToViewport(0);
}
