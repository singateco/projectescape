// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(SYLog, Log, All);

#define  PRINTLOG(fmt, ... ) \
UE_LOG(SYLog, Warning, TEXT("[%s]%s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))