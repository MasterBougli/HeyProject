// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ALSEnumLibrary.h"
#include "ALSStructLibrary.generated.h"

class UCurveVector;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FALSMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0), Category = "Movement Settings")
	float WalkSpeed { 175.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0), Category = "Movement Settings")
	float RunSpeed { 375.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0), Category = "Movement Settings")
	float SprintSpeed { 650.0f };

	// Gait amount to acceleration, deceleration and ground friction curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	UCurveVector* MovementCurve;

	// Gait amount to rotation speed curve.
	// Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Settings")
	UCurveFloat* RotationRateCurve;

	float GetSpeedForGait(const EALSGait Gait) const
	{
		switch (Gait)
		{
		case EALSGait::Running:
			return RunSpeed;
			
		case EALSGait::Sprinting:
			return SprintSpeed;
			
		case EALSGait::Walking:
			return WalkSpeed;
			
		default:
			checkNoEntry();
				return 0.0f;
		}
	}
};
