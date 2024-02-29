// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GrabComponent.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Enemy/RifleEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "Player/PhysicsHandleComp.h"
#include "Math/UnrealMathUtility.h"
#include "Objects/PickableActor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemy/EnemyBullet.h"
#include "Enemy/GrenadeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectEscape/ProjectEscape.h"
#include "Particles/ParticleSystem.h"
#include "System/ProjectEscapePlayerController.h"
#include "UI/MainUI.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy/EnemyBase.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent=true;

	// ...



	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectFinder{ TEXT( "/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'" ) };
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectFinder{ TEXT( "/Script/Engine.ParticleSystem'/Game/Resources/KDE/BulletImpactVFX/BulletHitVFX_SFX/AParticleSystem/PS_BulletHit_Tinplate.PS_BulletHit_Tinplate'" ) };
	

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/ExplosionsMegaPack/Niagara/Snow/NS_Explosion_Snow_4.NS_Explosion_Snow_4'" ) };
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ExplosionEffectFinder{ TEXT( "/Script/Niagara.NiagaraSystem'/Game/Resources/KDE/Blood_VFX/VFX/Performance_Versions/Bullet_Hits/One_Shot/OS_NS_Bullet_Hit_Medium.OS_NS_Bullet_Hit_Medium'" ) };

	if ( ExplosionEffectFinder.Succeeded() )
	{
		QExplosionEffect = ExplosionEffectFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UInputAction> GrabActionFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Grab.IA_Grab'")};
	if (GrabActionFinder.Succeeded())
	{
		ActionGrab = GrabActionFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UInputAction> QSkillActionFinder {TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_QUltimateSkill.IA_QUltimateSkill'")};
	if (QSkillActionFinder.Succeeded())
	{
		InputActionQSkill = QSkillActionFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UAnimMontage> GrabMontageFinder {TEXT("/Script/Engine.AnimMontage'/Game/Animations/Grab/GrabMontage.GrabMontage'")};
	if (GrabMontageFinder.Succeeded())
	{
		GrabbingMontage = GrabMontageFinder.Object;
	}

	static const ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowMontageFinder {TEXT("/Script/Engine.AnimMontage'/Game/Animations/Grab/Throwing.Throwing'")};
	if (ThrowMontageFinder.Succeeded())
	{
		ThrowingMontage = ThrowMontageFinder.Object;
	}
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//OtherEnemies.Empty();
	//
	//for ( TActorIterator<AEnemyBase> it( GetWorld() ); it; ++it ) {
	//	
	//	OtherEnemies.Add(*it);
	//	it->EnemyHPComponent->SetVisibility(false);
	//}
	AnimInstance = Player->GetMesh()->GetAnimInstance();
}

void UGrabComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player=GetOwner<AProjectEscapePlayer>();
	check( Player );
	HandleObject = Player->PhysicsHandleComponent;

	PC=Cast<AProjectEscapePlayerController>( GetWorld()->GetFirstPlayerController() );
}

// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//float TargetSpeed = 50.0f;
	//NewInterpolSpeed = FMath::FInterpTo( NewInterpolSpeed, TargetSpeed, DeltaTime, 0.5 );

	HandleObject->SetInterpolationSpeed( NewInterpolSpeed );


	//TArray<AActor*> AllPickUpActors;
	//UGameplayStatics::GetAllActorsOfClass( GetWorld(), APickableActor::StaticClass(), AllPickUpActors );


	for ( TActorIterator<APickableActor> it( GetWorld() ); it; ++it ) {
		AActor* OtherActor=*it;

		it->MeshComp->SetRenderCustomDepth( false );
	}

	OtherEnemies.Empty();
	
	for ( TActorIterator<AEnemyBase> it( GetWorld() ); it; ++it ) {
		
		OtherEnemies.Add(*it);
		it->EnemyHPComponent->SetVisibility(false);
	}



	TArray<FHitResult> HitInfoArrayPickUpActors;
	TArray<FHitResult> HitInfoArrayEnemies;
	FHitResult HitInfoPickUpActor;
	FVector Start=Player->GetFollowCamera()->GetComponentLocation();
	FVector End=Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * EnemyHPMaxDistance;


	ETraceTypeQuery TraceTypeQuery=UEngineTypes::ConvertToTraceType( ECC_GameTraceChannel1 );
	ETraceTypeQuery TraceTypeQueryEnemy=UEngineTypes::ConvertToTraceType( ECC_GameTraceChannel2 );

	TArray<AActor*> SphereTraceIgnoreActorsArray;
	// 체크하기전에 한 번 비워주기
	SphereTraceIgnoreActorsArray.Empty();
	// 자기 자신 캐릭터 무시
	SphereTraceIgnoreActorsArray.Add( Player );



	if( OtherEnemies.Num() > 0 )
	{
		/*bool bTraceResultEnemies=UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetectionHPBar, TraceTypeQueryEnemy,
			false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::ForDuration, HitInfoArrayEnemies, true, FColor::Green, FColor::Red, 2.0f );*/
		bool bTraceResultEnemies=UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetectionHPBar, TraceTypeQueryEnemy,
			false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArrayEnemies, true);
		if ( bTraceResultEnemies )
		{
			for ( FHitResult& HitInfo : HitInfoArrayEnemies )
			{
				if ( HitInfo.GetActor()->IsA<AEnemyBase>() )
				{
					auto TargetEnemy=Cast<AEnemyBase>( HitInfo.GetActor() );
					//UE_LOG( SYLog, Warning, TEXT( "TargetEnemy %s" ), *TargetEnemy->GetActorNameOrLabel() );
					if ( TargetEnemy )
					{
						TargetEnemy->EnemyHPComponent->SetVisibility( true );
					}
				}
			}
			TArray<AActor*> ResultEnemies;
			for(auto temp : HitInfoArrayEnemies )
			{
				ResultEnemies.Add( temp .GetActor());
			}

			//시야에 없는 에너미 HP바 지우기
			//if ( OtherEnemies.Num() > HitInfoArrayPickUpActors.Num() )
			//{
			//	for ( auto anyEnemy : OtherEnemies )
			//	{
			//		//if( !ResultEnemies.Contains(a) ) //맵에 있는 전체 에너미 중에서 레이더에 잡힌 에너미들에 포함되지않는다면 
			//		//{
			//		//	a->EnemyHPComponent->SetVisibility( false );
			//		//
			//		//}
			//
			//		for ( auto b : HitInfoArrayPickUpActors )
			//		{
			//			if ( b.GetActor()->GetActorNameOrLabel() != anyEnemy->GetActorNameOrLabel() )
			//			{
			//				auto tempEnemy=Cast<AEnemyBase>( anyEnemy );
			//				tempEnemy->EnemyHPComponent->SetVisibility( false );
			//			}
			//		}
			//	}
			//}

			//시야에 없는 에너미 HP바 지우기
			if ( OtherEnemies.Num() > HitInfoArrayEnemies.Num() )
			{
				for ( auto a : OtherEnemies )
				{
					for ( auto b : HitInfoArrayEnemies )
					{
						if ( b.GetActor()->GetActorNameOrLabel() != a->GetActorNameOrLabel() )
						{
							auto aa=Cast<AEnemyBase>( a );
							aa->EnemyHPComponent->SetVisibility( false );
						}
					}
				}
			}



		}
	}

	if(bIsGrabbing == true)
	{
		HandleObject->SetTargetLocation( Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector()*500 );
		//HandleObject->SetTargetLocation( Player->GetMesh()->GetSocketLocation("GrabPosition") );
		//NewAngle+= RotSpeed * DeltaTime;
		//FRotator NewRotation = FRotator( 0, NewAngle, 0 );
		//FRotator NewRotation = FRotator( NewAngle, NewAngle, NewAngle );
		//HandleObject->SetTargetRotation( NewRotation );
		//HandleObject->SetAngularDamping(10*DeltaTime);

		//FVector NewLocation =Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * 500;
		//FRotator NewRotation = FRotator( 10 * DeltaTime, 10 * DeltaTime, 10 * DeltaTime );
		//HandleObject->SetTargetLocationAndRotation( NewLocation, NewRotation );

		if ( AnimInstance && GrabbingMontage && !AnimInstance->Montage_IsPlaying( nullptr ) )
		{
			AnimInstance->Montage_Play( GrabbingMontage );
		}
		
	}else if( bIsGrabbing == false || OtherEnemies.Num() > 0) // 물체 안잡고 있거나 적들이 있을 때
	{
		//bool bTracePickUpActorSphereMulti = UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetection, TraceTypeQuery, false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArrayPickUpActors, true );
		bool bTracePickUpActorSphereSingle = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End , RadiusDetection, TraceTypeQuery , false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoPickUpActor, true );

		if ( ESkillCurrentCoolTime < ESkillMaxCoolTime )
		{
			return;
		}

		if ( bTracePickUpActorSphereSingle )
		{
			APickableActor* OutlineObject =  Cast<APickableActor>( HitInfoPickUpActor.GetActor() );
			if( OutlineObject )
			{
				PickUpActorResult = HitInfoPickUpActor;
				OutlineObject->MeshComp->SetRenderCustomDepth( true );
				ObjectInHand = OutlineObject;
			}
			else // if(OutlineObject == nullptr)
			{
				//잡을 물건 없음
				//Spawn Actor Fracture
			}
		}

		/*
		if ( bTracePickUpActorSphereSingle )
		{
			//UE_LOG( SYLog, Warning, TEXT( "OtherEnemies %d" ), OtherEnemies.Num() );
		
			for ( FHitResult& HitInfo : HitInfoArrayPickUpActors )
			{
				//auto PickUpActor=Cast<APickableActor>( HitInfo.GetActor() );
				////UE_LOG( SYLog, Warning, TEXT( "%s" ), *PickUpActor->GetActorNameOrLabel() );
				//if(PickUpActor )
				//{
				//	PickUpActor->MeshComp->SetRenderCustomDepth( true );
				//}

				if(HitInfo.GetActor()->IsA<APickableActor>())
				{
					auto PickUpActor=Cast<APickableActor>( HitInfo.GetActor() );
					//UE_LOG( SYLog, Warning, TEXT( "PickUpActor %s" ), *PickUpActor->GetActorNameOrLabel() );
					if ( PickUpActor )
					{
						PickUpActor->MeshComp->SetRenderCustomDepth( true );
					}
				
				}
				//if( HitInfo.GetActor()->IsA<ARifleEnemy>() || HitInfo.GetActor()->IsA<AGrenadeEnemy>() )
				//if( HitInfo.GetActor()->IsA<AEnemyBase>() )
				//{
				//	auto TargetEnemy = Cast<AEnemyBase>( HitInfo.GetActor() );
				//	UE_LOG( SYLog, Warning, TEXT( "TargetEnemy %s" ), *TargetEnemy->GetActorNameOrLabel() );
				//	if(TargetEnemy )
				//	{
				//		TargetEnemy->EnemyHPComponent->SetVisibility( true );
				//
				//	}
				//	
				//}
			}

			//시야에 없는 에너미 HP바 지우기
			//if( OtherEnemies.Num()> HitInfoArrayPickUpActors.Num() )
			//{
			//	for(auto a : OtherEnemies )
			//	{
			//		for(auto b: HitInfoArrayPickUpActors )
			//		{
			//			if(b.GetActor()->GetActorNameOrLabel() != a->GetActorNameOrLabel() )
			//			{
			//				auto aa = Cast<AEnemyBase>(a);
			//				aa->EnemyHPComponent->SetVisibility(false);
			//			}
			//		}
			//	}
			//}
		}
*/
	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = PlayerInputComponent;
	
	PlayerInputComponent->BindAction(ActionGrab, ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	//PlayerInputComponent->BindAction(ActionGrab, ETriggerEvent::Started, this, &UGrabComponent::SphereGrabObject);
	PlayerInputComponent->BindAction( ActionGrab, ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject );
	PlayerInputComponent->BindAction( InputActionQSkill, ETriggerEvent::Completed, this, &UGrabComponent::ActionQSkill );
}

void UGrabComponent::GrabObject()
{
	if(bIsGrabbing == true)
	{
		return;
	}

	bIsPulling = true;
	bIsPushing = false;


	if ( ESkillCurrentCoolTime < ESkillMaxCoolTime )
	{
		return;
	}

	if ( ObjectInHand )
	{

		UE_LOG( SYLog, Warning, TEXT( "pick!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! : %s" ), *ObjectInHand->GetActorNameOrLabel() );
		if(PickUpActorResult.GetActor() == nullptr || PickUpActorResult.GetComponent() == nullptr || !PickUpActorResult.HasValidHitObjectHandle() )
		{
			return;
		}
		if( PickUpActorResult.HasValidHitObjectHandle())
		{

			HandleObject->GrabComponentAtLocation( PickUpActorResult.GetComponent(), TEXT( "GrabObject" ), PickUpActorResult.GetComponent()->GetComponentLocation() );
			//HandleObject->SetAngularDamping( 0 );
			if ( HandleObject->GetGrabbedComponent() != nullptr )
			{
				bIsGrabbing=true;
			}
		}
	}
	else // if(OutlineObject == nullptr)
	{
		UE_LOG( SYLog, Warning, TEXT( "There is no pickable actor!"));
		//잡을 물건 없음
		//Spawn Actor Fracture
	}
}

void UGrabComponent::ReleaseObject()
{
	if ( bIsGrabbing == false ) return;

	bIsPulling=false;
	bIsPushing=true;

	if(bIsGrabbing == true )
	{
		/********************************** Sphere Trace SingleByChannel **********************************/
		/**************************************************************************************************/

		TArray<FHitResult> HitInfoArray;
		FVector Start=Player->GetFollowCamera()->GetComponentLocation();
		FVector End=Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;


		ETraceTypeQuery TraceTypeQuery=UEngineTypes::ConvertToTraceType( ECC_GameTraceChannel2 );

		TArray<AActor*> SphereTraceIgnoreActorsArray;
		// 체크하기전에 한 번 비워주기
		SphereTraceIgnoreActorsArray.Empty();
		// 자기 자신 캐릭터 무시
		SphereTraceIgnoreActorsArray.Add( Player );

		bool bTraceResult=UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetectionEnemy, TraceTypeQuery,
			false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArray, true );


		/**************************************************************************************************/
		/**************************************************************************************************/
		ThrowingLoc = Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;

		if ( bTraceResult )
		{
			for ( FHitResult& HitInfo : HitInfoArray )
			{
				auto Enemy =Cast<AEnemyBase>( HitInfo.GetActor() );

				//UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Enemies->GetActorNameOrLabel() )

					if ( Enemy )
					{
						UE_LOG( SYLog, Warning, TEXT( "EnemyLoc throw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ) )
						FVector EnemyLoc = Enemy->GetActorLocation();
						ThrowingLoc = EnemyLoc;
						break;
					}
			}
		}else
		{
			UE_LOG( SYLog, Warning, TEXT( "CenterLoc throw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ) )
		}

		//FVector ThrowingDirection = HandleObject->GetGrabbedComponent()->GetComponentLocation() - ThrowingLoc;
		FVector ThrowingDirection = ThrowingLoc - HandleObject->GetGrabbedComponent()->GetComponentLocation();
		ThrowingDirection.Normalize();
		HandleObject->GetGrabbedComponent()->AddImpulse( ThrowingDirection * ThrowingPower, NAME_None, true);
		HandleObject->ReleaseComponent();

		bIsGrabbing=false;

		if (AnimInstance && ThrowingMontage)
		{
			if (GrabbingMontage)
			{
				AnimInstance->Montage_Stop(0, GrabbingMontage);
			}
			AnimInstance->Montage_Play(ThrowingMontage);
		}
	}
	GetWorld()->GetTimerManager().SetTimer( ESkillCountDownHandle, this, &UGrabComponent::ESkillAdvanceTimer, 1.0f, true );

}

void UGrabComponent::ActionQSkill()
{

	if ( GetWorld()->GetTimerManager().IsTimerActive( QSkillCountDownHandle )) {
		return;
	}

	GetWorld()->GetTimerManager().SetTimer( QSkillCountDownHandle, this, &UGrabComponent::QSkillAdvanceTimer, 1.0f, true );

	//UGameplayStatics::SpawnEmitterAtLocation( GetWorld(),QExplosionEffect, Player->GetActorLocation(), FRotator(), FVector( 100 ), true, EPSCPoolMethod::None, true );
	UNiagaraFunctionLibrary::SpawnSystemAtLocation( GetWorld(), QExplosionEffect, Player->GetActorLocation(), FRotator(), FVector( 5 ), true);
}

void UGrabComponent::SphereGrabObject()
{
	if ( bIsGrabbing == true )
	{
		return;
	}

	if(GetWorld()->GetTimerManager().IsTimerActive(ESkillCountDownHandle)){
		return;
	}
	/********************************** Sphere Trace SingleByChannel **********************************/
	/**************************************************************************************************/

	UE_LOG( SYLog, Warning, TEXT( "pick!!!!!!!!!!!!!!! " ) );
	TArray<FHitResult> HitInfoArray;
	FVector Start=Player->GetFollowCamera()->GetComponentLocation();
	FVector End=Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;
	

	ETraceTypeQuery TraceTypeQuery=UEngineTypes::ConvertToTraceType( ECC_GameTraceChannel1 );

	TArray<AActor*> SphereTraceIgnoreActorsArray;
	// 체크하기전에 한 번 비워주기
	SphereTraceIgnoreActorsArray.Empty();
	// 자기 자신 캐릭터 무시
	SphereTraceIgnoreActorsArray.Add( Player );

	bool bTraceResult = UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetection, TraceTypeQuery,
		false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::ForDuration, HitInfoArray, true, FColor::Purple, FColor::Red, 2.0f );


	/**************************************************************************************************/
	/**************************************************************************************************/
	//TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	//objectType.Emplace( UEngineTypes::ConvertToObjectType( ECC_GameTraceChannel1 ) );

	//bool bTraceResult2 = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, RadiusDetection, objectType,  false, SphereTraceIgnoreActorsArray , EDrawDebugTrace::ForDuration, HitInfoArray, true );
	if( bTraceResult )
	{

		for ( FHitResult& HitInfo : HitInfoArray )
		{
			auto PickUpActor=Cast<APickableActor>( HitInfo.GetActor() );

			if ( PickUpActor )
			{
				UE_LOG( SYLog, Warning, TEXT( "pick!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! : %s" ), *PickUpActor->GetActorNameOrLabel() );
				PickUpActor->MeshComp->SetRenderCustomDepth( true );
				HandleObject->GrabComponentAtLocation( HitInfo.GetComponent(), TEXT( "GrabObject" ), HitInfo.GetComponent()->GetComponentLocation() );
				
				if ( HandleObject->GetGrabbedComponent() != nullptr )
				{
					bIsGrabbing=true;
				}
			}
		}
		//GetWorld()->GetTimerManager().SetTimer( ESkillCountDownHandle, this, &UGrabComponent::ESkillAdvanceTimer, 1.0f, true );
	}
}

void UGrabComponent::QSkillAdvanceTimer()
{
	--QSkillCurrentCoolTime;

	QSkillUpdateTimerDisplay();

	if ( QSkillCurrentCoolTime < 1 ) {
		// 카운트 다운이 완료 되었으니 타이머를 중지 시킨다.
		GetWorld()->GetTimerManager().ClearTimer( QSkillCountDownHandle );

		PC->InGameWIdget->EndQSkillUI();
		QSkillCurrentCoolTime=QSkillMaxCoolTime;
	}
}

void UGrabComponent::ESkillAdvanceTimer()
{
	ESkillUpdateTimerDisplay();
	--ESkillCurrentCoolTime;


	if(ESkillCurrentCoolTime<1){
		// 카운트 다운이 완료 되었으니 타이머를 중지 시킨다.
		GetWorld()->GetTimerManager().ClearTimer( ESkillCountDownHandle );

		PC->InGameWIdget->EndESkillUI();
		ESkillCurrentCoolTime = ESkillMaxCoolTime;
	}
}

void UGrabComponent::ESkillUpdateTimerDisplay()
{
	if ( PC == nullptr ) {
		return;
	}
	PC->InGameWIdget->StartESkillUI();
	PC->InGameWIdget->SetESkillCoolTimer(ESkillCurrentCoolTime,ESkillMaxCoolTime);
}

void UGrabComponent::QSkillUpdateTimerDisplay()
{
	if ( PC == nullptr ) {
		return;
	}
	PC->InGameWIdget->StartQSkillUI();
	PC->InGameWIdget->SetQSkillCoolTimer( QSkillCurrentCoolTime, QSkillMaxCoolTime );
}

void UGrabComponent::Deactivate()
{
	Super::Deactivate();

	EnhancedInputComponent->ClearBindingsForObject(this);
}
