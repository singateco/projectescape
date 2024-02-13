// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GrabComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "ProjectEscape/Public/Player/ProjectEscapePlayer.h"
#include "Player/PhysicsHandleComp.h"
#include "Math/UnrealMathUtility.h"

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

	float TargetSpeed = 50.0f;
	NewInterpolSpeed = FMath::FInterpTo( NewInterpolSpeed, TargetSpeed, DeltaTime, 0.5 );

	HandleObject->SetInterpolationSpeed( NewInterpolSpeed );

	if(bIsGrabbing == true)
	{
		HandleObject->SetTargetLocation( Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector()*500 );
		NewAngle+= 12000 * DeltaTime;
		FRotator NewRotation = FRotator( 0, NewAngle, 0 );
		HandleObject->SetTargetRotation( NewRotation );
		//HandleObject->SetAngularDamping(10*DeltaTime);


		//FVector NewLocation =Player->GetFollowCamera()->GetComponentLocation() + Player->GetFollowCamera()->GetForwardVector() * 500;
		//FRotator NewRotation = FRotator( 10 * DeltaTime, 10 * DeltaTime, 10 * DeltaTime );
		//HandleObject->SetTargetLocationAndRotation( NewLocation, NewRotation );
	}
}


void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(ActionGrab, ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	PlayerInputComponent->BindAction( ActionGrab, ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject );
}

void UGrabComponent::GrabObject()
{
	if(bIsGrabbing == true)
	{
		return;
	}

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
		NewInterpolSpeed = 0.0f;

		//HandleObject->GrabComponentAtLocationWithRotation( HitInfo.GetComponent(), TEXT("GrabObject"),HitInfo.GetComponent()->GetComponentLocation(), HitInfo.GetComponent()->GetComponentRotation());
		HandleObject->GrabComponentAtLocation( HitInfo.GetComponent(), TEXT("GrabObject"),HitInfo.GetComponent()->GetComponentLocation());

		//HandleObject->SetAngularDamping( 0 );


		//HandleObject->SetInterpolationSpeed(NewInterpolSpeed);

		if(HandleObject->GetGrabbedComponent() != nullptr )
		{
			bIsGrabbing = true;
		}
	}


}

void UGrabComponent::ReleaseObject()
{
	if(bIsGrabbing == true )
	{
		HandleObject->ReleaseComponent();

		bIsGrabbing=false;
		
	}
}