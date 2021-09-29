// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ALSEnumLibrary.generated.h"

/**
 * Character gait state. Note: Also edit related struct in ALSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class EALSGait : uint8
{
	Walking,
	Running,
	Sprinting
};
