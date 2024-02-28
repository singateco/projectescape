// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireComponent.generated.h"

class AEnemyBase;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class AProjectEscapePlayer;
class ANormalGun;
class UMainUI;
class UParticleSystem;
class AProjectEscapePlayerController;
class UNiagaraSystem;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyHitByPlayerGun, AEnemyBase*, Enemy, FHitResult, HitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireComponent();
	
	UPROPERTY()
	AProjectEscapePlayer* Player;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	ANormalGun* NormalGun;
	
	FEnemyHitByPlayerGun OnEnemyHitByPlayerGun;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxDistanceToGun = 100000.0f;

	UPROPERTY( EditDefaultsOnly, Category="Weapon" )
	FVector FireEffectScale = FVector(10);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bHasPistol = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActionFire;


	UPROPERTY( EditDefaultsOnly, Category="Input" )
	UInputAction* ActionReload;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ActionAimDownSight;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ANormalGun> NormalGunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	UNiagaraSystem* GunUpgradeEffect;

	UPROPERTY( EditAnywhere, Category="Pistol" )
	UMaterialInterface* WallDecalEffect;

	UPROPERTY( EditAnywhere, Category="Pistol" )
	FVector WallDecalScale = FVector(10);;

	UPROPERTY( EditDefaultsOnly, Category="Pistol" )
	UNiagaraSystem* BloodEffect;

	UPROPERTY( EditDefaultsOnly, Category="Pistol" )
	UNiagaraSystem* GunEffectNoActor;


	UPROPERTY( EditDefaultsOnly, Category="Pistol" )
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Pistol")
	float GunImpulseForce {15000.f};

	UPROPERTY( EditDefaultsOnly, Category="Pistol" )
	USoundBase* GunSoundClass;

	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY(EditAnywhere, Category = "Settings|Recoil")
	float RecoilValueMin {.15};

	UPROPERTY(EditAnywhere, Category = "Settings|Recoil")
	float RecoilValueMax {.4};

	UPROPERTY(EditAnywhere, Category = "Settings|ADS")
	FVector AdsOffset = FVector(-10, -60, 20);


	virtual void InitializeComponent() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent) ;

	//void Fire();

	void NormalGunFire();

	void AttachPistol();

	void DetachPistol();
	
	void StartAimDown(const FInputActionInstance& InputActionInstance);

	void EndAimDown(const FInputActionInstance& InputActionInstance);


	void BulletReload();

	void InitBullets();

	// ############# Animations ##############

	// 전 프레임에 총 발사했는지 여부
	UPROPERTY(BlueprintReadWrite)
	bool bHasFired {false};

	// 총 발사시 플레이되는 몽타주
	UPROPERTY(EditAnywhere, Category="MyAnimSettings" )
	UAnimMontage* FireMontage;
	
	//Normal Gun Reload
	UPROPERTY( EditDefaultsOnly, Category="MyAnimSettings" )
	UAnimMontage* ReloadMontage;

	UFUNCTION()
	void HandleFireAnimation();


	UFUNCTION()
	void PlayReloadAnimation();

	virtual void Deactivate() override;

	UPROPERTY( EditAnywhere )
	AProjectEscapePlayerController* PC;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};
