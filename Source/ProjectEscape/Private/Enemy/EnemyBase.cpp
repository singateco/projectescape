// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

#include "NavigationInvokerComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyBaseFSM.h"
#include "Enemy/EnemyHealthBar.h"
#include "Enemy/EnemyStatsComponent.h"
#include "Objects/HealthPickup.h"
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
	NavComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavComponent"));

	WeakPoint = CreateDefaultSubobject<UBoxComponent>( TEXT( "WeakPoint" ) );
	WeakPoint->SetupAttachment( GetMesh(), TEXT("WeakPointSocket") );
	WeakPoint->SetBoxExtent(FVector(16.f));
	WeakPoint->SetCollisionObjectType(ECC_GameTraceChannel4);
	WeakPoint->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeakPoint->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunMesh" ) );
	GunMesh->SetupAttachment( GetMesh(), FName( TEXT( "RightHandSocket" ) ) );

	EnemyHPComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHPComponent"));
	EnemyHPComponent->SetupAttachment(RootComponent);
	
	//WBP(블루프린트 클래스)를 로드해서 HPComp의 위젯으로 설정, FClassFinder 주소 마지막에 _C해야함 블루프린트라서
	ConstructorHelpers::FClassFinder<UUserWidget> tempHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_EnemyHealthBar.WBP_EnemyHealthBar_C'"));

	if (tempHP.Succeeded())
	{
		EnemyHPComponent->SetWidgetClass(tempHP.Class);
		EnemyHPComponent->SetWidgetSpace(EWidgetSpace::Screen);
		EnemyHPComponent->SetDrawSize(FVector2D(70, 10));
		EnemyHPComponent->SetRelativeLocation(FVector(0, 0, 100));
		EnemyHPComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> DamageNumberWidgetFinder {TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_DamageNumber.WBP_DamageNumber_C'")};

	if (DamageNumberWidgetFinder.Succeeded())
	{
		DamageNumberWidgetClass = DamageNumberWidgetFinder.Class;
	}

	ConstructorHelpers::FClassFinder<AHealthPickup> HealthPickupBPFinder {TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_HealthPickup.BP_HealthPickup_C'")};

	if (HealthPickupBPFinder.Succeeded())
	{
		HealthPickupActorClass = HealthPickupBPFinder.Class;
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	UCapsuleComponent* cap = GetCapsuleComponent();
	auto mesh = GetMesh();

	cap->SetCollisionProfileName( FName( "Enemy") );
	mesh->SetCollisionProfileName( FName( "NoCollision" ) );

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed=EnemyMaxSpeed;

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	check(DamageNumberWidgetClass);
	StatsComponent->OnHPReachedZero.AddUniqueDynamic(this, &AEnemyBase::ProcessDying);
	StatsComponent->OnTakenDamage.AddUniqueDynamic(this, &AEnemyBase::DisplayDamageNumber);
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FVector Start = EnemyHPComponent->GetComponentLocation();
	FVector End = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	float DistanceToPlayer = FVector::Distance( Start, End );
	float MaxDisplayDistance = 3000.0f;

	//if(DistanceToPlayer <= MaxDisplayDistance )
	//{
	//	EnemyHPComponent->SetVisibility( true );
	//}
	//else
	//{
	//	EnemyHPComponent->SetVisibility( false );
	//}

	FRotator newRoatation = UKismetMathLibrary::FindLookAtRotation(Start, End);
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

void AEnemyBase::ProcessDying()
{
	OnEnemyDied.Broadcast(this);

	if (HealthPickupActorClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AHealthPickup>(HealthPickupActorClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	}
}

void AEnemyBase::DisplayDamageNumber(const float DamageToDisplay)
{
	UDamageNumber* DamageWidget = Cast<UDamageNumber>(CreateWidget(GetWorld(), DamageNumberWidgetClass));
	DamageWidget->Actor = this;
	DamageWidget->DamageToDisplay = DamageToDisplay;
	DamageWidget->AddToViewport(0);
}

