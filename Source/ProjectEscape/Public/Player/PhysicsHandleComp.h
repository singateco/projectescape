// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PhysicsHandleComp.generated.h"

/**
 * 
 */
UCLASS()
class UPhysicsHandleComp : public UPhysicsHandleComponent
{
	GENERATED_BODY()
public:

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
};
