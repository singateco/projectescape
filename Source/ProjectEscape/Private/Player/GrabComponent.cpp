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
#include "Kismet/GameplayStatics.h"
#include "ProjectEscape/ProjectEscape.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent=true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGrabComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Player=GetOwner<AProjectEscapePlayer>();
	check( Player );
	HandleObject = Player->PhysicsHandleComponent;
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
	}else
	{

		TArray<FHitResult> HitInfoArray;
		FVector Start=Player->GetFollowCamera()->GetComponentLocation();
		FVector End=Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;


		ETraceTypeQuery TraceTypeQuery=UEngineTypes::ConvertToTraceType( ECC_GameTraceChannel1 );

		TArray<AActor*> SphereTraceIgnoreActorsArray;
		// 체크하기전에 한 번 비워주기
		SphereTraceIgnoreActorsArray.Empty();
		// 자기 자신 캐릭터 무시
		SphereTraceIgnoreActorsArray.Add( Player );

		bool bTraceResult=UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetection, TraceTypeQuery,
			false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArray, true );


		if ( bTraceResult )
		{
			for ( FHitResult& HitInfo : HitInfoArray )
			{
				auto PickUpActor=Cast<APickableActor>( HitInfo.GetActor() );
				//UE_LOG( SYLog, Warning, TEXT( "%s" ), *PickUpActor->GetActorNameOrLabel() );
				if(PickUpActor )
				{
					PickUpActor->MeshComp->SetRenderCustomDepth( true );
				}
			}

		}
	}
}


void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = PlayerInputComponent;
	
	//PlayerInputComponent->BindAction(ActionGrab, ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	PlayerInputComponent->BindAction(ActionGrab, ETriggerEvent::Started, this, &UGrabComponent::SphereGrabObject);
	PlayerInputComponent->BindAction( ActionGrab, ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject );
}

void UGrabComponent::GrabObject()
{
	if(bIsGrabbing == true)
	{
		return;
	}

	bIsPulling = true;
	bIsPushing = false;

	// 1. Collision Check - LineTrace 1st
	FHitResult HitInfo;
	// 1) From Crosshair
	FVector StartPos=Player->GetFollowCamera()->GetComponentLocation();
	// 2) To End Point(Max Distance)
	FVector EndPos=Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor( Player );

	bool bHit=GetWorld()->LineTraceSingleByChannel( HitInfo, StartPos, EndPos, ECC_Visibility, Params );
	DrawDebugLine( GetWorld(), StartPos, EndPos, FColor::Red, true );
	if( bHit )
	{
		//HandleObject->GrabComponentAtLocationWithRotation( HitInfo.GetComponent(), TEXT("GrabObject"),HitInfo.GetComponent()->GetComponentLocation(), HitInfo.GetComponent()->GetComponentRotation());
		HandleObject->GrabComponentAtLocation( HitInfo.GetComponent(), TEXT("GrabObject"),HitInfo.GetComponent()->GetComponentLocation());

		//HandleObject->SetAngularDamping( 0 );
		if(HandleObject->GetGrabbedComponent() != nullptr )
		{
			bIsGrabbing = true;
		}
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

		bool bTraceResult=UKismetSystemLibrary::SphereTraceMulti( GetWorld(), Start, End, RadiusDetection, TraceTypeQuery,
			false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArray, true );


		/**************************************************************************************************/
		/**************************************************************************************************/
		ThrowingLoc = Player->GetFollowCamera()->GetForwardVector() * MaxDistanceToGrab;
		
		if ( bTraceResult )
		{
			UE_LOG( SYLog, Warning, TEXT( "throw!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" ))
			for ( FHitResult& HitInfo : HitInfoArray )
			{
				auto Enemy =Cast<ARifleEnemy>( HitInfo.GetActor() );

				//UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Enemies->GetActorNameOrLabel() )

					if ( Enemy )
					{
						FVector EnemyLoc = Enemy->GetActorLocation();
						ThrowingLoc = EnemyLoc;
						break;
					}
			}
		}

		//FVector ThrowingDirection = HandleObject->GetGrabbedComponent()->GetComponentLocation() - ThrowingLoc;
		FVector ThrowingDirection = ThrowingLoc - HandleObject->GetGrabbedComponent()->GetComponentLocation();
		ThrowingDirection.Normalize();
		HandleObject->GetGrabbedComponent()->AddImpulse( ThrowingDirection * ThrowingPower, NAME_None, true);
		HandleObject->ReleaseComponent();

		bIsGrabbing=false;
	}
}

void UGrabComponent::SphereGrabObject()
{
	if ( bIsGrabbing == true )
	{
		return;
	}

	/********************************** Sphere Trace SingleByChannel **********************************/
	/**************************************************************************************************/

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
		false, SphereTraceIgnoreActorsArray, EDrawDebugTrace::None, HitInfoArray, true );


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
			UE_LOG(SYLog, Warning, TEXT("pick!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! : %s"), *PickUpActor->GetActorNameOrLabel());
			PickUpActor->MeshComp->SetRenderCustomDepth(true);


			if ( PickUpActor )
			{
				HandleObject->GrabComponentAtLocation( HitInfo.GetComponent(), TEXT( "GrabObject" ), HitInfo.GetComponent()->GetComponentLocation() );

				if ( HandleObject->GetGrabbedComponent() != nullptr )
				{
					bIsGrabbing=true;
				}
			}

		}


	}
}

void UGrabComponent::Deactivate()
{
	Super::Deactivate();

	EnhancedInputComponent->ClearBindingsForObject(this);
}
