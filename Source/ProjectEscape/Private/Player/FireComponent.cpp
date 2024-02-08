// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FireComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animations/ProjectEscapeAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "Weapon/NormalGun.h"

// Sets default values for this component's properties
UFireComponent::UFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UFireComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	check(NormalGunClass);
	NormalGun = GetWorld()->SpawnActor<ANormalGun>(NormalGunClass);
	AttachPistol();
}

void UFireComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player = GetOwner<AProjectEscapePlayer>();
	check(Player);

	
}


// Called every frame
void UFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFireComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(ActionFire, ETriggerEvent::Started, this, &UFireComponent::NormalGunFire);
}

void UFireComponent::NormalGunFire()
{

	// ����ó�� : ���̾����� , ����, �� ������ ���, ���¹̳� ���� ��  
	if (bHasPistol == false ) {
		return;
	}

	//FRotator GazeRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), Player->GetCameraBoom()->GetForwardVector() * MaxDistanceToGun);
	//Player->SetActorRotation(GazeRotation);

	// �ѽ��
	FHitResult HitInfo;
	FVector StartPos = NormalGun->NormalGunMesh->GetSocketLocation(TEXT("Muzzle"));
	FVector EndPos = Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector()*MaxDistanceToGun;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPos, EndPos,ECC_Visibility, Params);

	if (bHit) { // �浹�ϸ�
		//���� �ڸ��� ��ƼŬ ȿ�� ���
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GunEffect, HitInfo.Location,FRotator());

		//if (Enemy) {
		//	Enemy->DamageProcess();
		//}
	}

	// ���׹̳� �Ҹ�

	//�ѽ�� �ִϸ��̼� ���
	//auto Anim = Cast<UProjectEscapeAnimInstance>(GetMesh()->GetAnimInstance());
	//Anim->PlayerFireAnimation();
}

void UFireComponent::AttachPistol()
{
	//Bullet ReLoad Animation
	bHasPistol = true;
	NormalGun->SetActorHiddenInGame(false);
	NormalGun->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunPosition"));
}


void UFireComponent::DetachPistol()
{
	bHasPistol = false;
	NormalGun->SetActorHiddenInGame(true);
	//NormalGun->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	//NormalGun->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}