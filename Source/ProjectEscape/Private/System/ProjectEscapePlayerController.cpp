﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ProjectEscapePlayerController.h"

#include "UI/MainUI.h"

void AProjectEscapePlayerController::BeginPlay()
{
	Super::BeginPlay();


	//WBP_Widget있는지 체크
	check( InGameWIdgetClass );

	InGameWIdget=CreateWidget<UMainUI>( this, InGameWIdgetClass, TEXT( "InGameWIdget" ) );
	//새로 만들어서 쓸게 아니라 이미 있는걸 갖다 써야될 것 같은데 어떻게 가져온담 .. 
	//OrderMenuWidget = CreateWidget<UOrderMenuWidget>( this , OrderMenuWidgetClass , TEXT( "OrderMenuWidget" ) );
	InGameWIdget->AddToViewport();

}