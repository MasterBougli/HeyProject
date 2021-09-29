// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#include "Components/ALSCharacterMovementComponent.h"
#include "Characters/ALSCharacterBase.h"
#include "Curves/CurveVector.h"

#pragma region Character Movement Component
UALSCharacterMovementComponent::UALSCharacterMovementComponent()
{

}

void UALSCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerOwner = Cast<AALSCharacterBase>(GetOwner());
}

float UALSCharacterMovementComponent::GetMaxAcceleration() const
{
	// Update the Acceleration using the Movement Curve.
	// This allows for fine control over movement behavior at each speed.
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxAcceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).X;
}

float UALSCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	// Update the Deceleration using the Movement Curve.
	// This allows for fine control over movement behavior at each speed.
	if (!IsMovingOnGround() || !CurrentMovementSettings.MovementCurve)
	{
		return Super::GetMaxBrakingDeceleration();
	}
	return CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Y;
}

FNetworkPredictionData_Client* UALSCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr);
	
	if (ClientPredictionData == nullptr)
	{
		UALSCharacterMovementComponent* MutableThis = const_cast<UALSCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FRPGNetworkPredictionData_Client_Character(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

bool UALSCharacterMovementComponent::IsCustomMovementMode(uint8 CustomMovementModeValue) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == CustomMovementModeValue;
}

void UALSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use. Below is what each is currently being used for:
		FLAG_Custom_0		= 0x10, // Sprinting
		FLAG_Custom_1		= 0x20, // Unused
		FLAG_Custom_2		= 0x40, // Unused
		FLAG_Custom_3		= 0x80, // Unused
	*/

	// Read the values from the compressed flags
	bRequestMovementSettingsChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UALSCharacterMovementComponent::PhysWalking(const float DeltaTime, const int32 Iterations)
{
	if (IsValid(CurrentMovementSettings.MovementCurve))
	{
		// Update the Ground Friction using the Movement Curve.
		// This allows for fine control over movement behavior at each speed.
		GroundFriction = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Z;
	}

	Super::PhysWalking(DeltaTime, Iterations);
}

void UALSCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (IsValid(CharacterOwner) && bRequestMovementSettingsChange)
	{
		MaxWalkSpeed = NewMaxWalkSpeed;
		MaxWalkSpeedCrouched = NewMaxWalkSpeed;
	}
}

float UALSCharacterMovementComponent::GetMappedSpeed() const
{
	// Map the character's current speed to the configured movement speeds with a range of 0-3,
	// with 0 = stopped, 1 = the Walk Speed, 2 = the Run Speed, and 3 = the Sprint Speed.
	// This allows us to vary the movement speeds but still use the mapped range in calculations for consistent results

	const float Speed = Velocity.Size2D();
	const float LocWalkSpeed = CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = CurrentMovementSettings.RunSpeed;
	const float LocSprintSpeed = CurrentMovementSettings.SprintSpeed;

	if (Speed > LocRunSpeed)
	{
		return FMath::GetMappedRangeValueClamped({LocRunSpeed, LocSprintSpeed}, {2.0f, 3.0f}, Speed);
	}

	if (Speed > LocWalkSpeed)
	{
		return FMath::GetMappedRangeValueClamped({LocWalkSpeed, LocRunSpeed}, {1.0f, 2.0f}, Speed);
	}

	return FMath::GetMappedRangeValueClamped({0.0f, LocWalkSpeed}, {0.0f, 1.0f}, Speed);
}

void UALSCharacterMovementComponent::SetMovementSettings(FALSMovementSettings NewMovementSettings)
{
	// Set the current movement settings from the owner
	CurrentMovementSettings = NewMovementSettings;
}

float UALSCharacterMovementComponent::GetSpeedForGait(const EALSGait Gait) const
{
	switch (Gait)
	{
		case EALSGait::Running:
			return CurrentMovementSettings.RunSpeed;
			
		case EALSGait::Sprinting:
			return CurrentMovementSettings.SprintSpeed;
			
		case EALSGait::Walking:
			return CurrentMovementSettings.WalkSpeed;
			
		default:
			checkNoEntry();
			return 0.0f;
	}
}

void UALSCharacterMovementComponent::SetMaxWalkingSpeed(float UpdateMaxWalkSpeed)
{
	if (UpdateMaxWalkSpeed != NewMaxWalkSpeed)
	{
		if (PawnOwner->IsLocallyControlled())
		{
			NewMaxWalkSpeed = UpdateMaxWalkSpeed;
			Server_SetMaxWalkingSpeed(UpdateMaxWalkSpeed);
			bRequestMovementSettingsChange = true;
			return;
		}
		if (!PawnOwner->HasAuthority())
		{
			MaxWalkSpeed = UpdateMaxWalkSpeed;
			MaxWalkSpeedCrouched = UpdateMaxWalkSpeed;
		}
	}
}

void UALSCharacterMovementComponent::Server_SetMaxWalkingSpeed_Implementation(float UpdateMaxWalkSpeed)
{
	NewMaxWalkSpeed = UpdateMaxWalkSpeed;
}

#pragma endregion Character Movement Component

#pragma region FSavedMove_Character
FRPGSavedMove_Character::FRPGSavedMove_Character()
{
	bSavedRequestMovementSettingsChange = false;
}

bool FRPGSavedMove_Character::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* InCharacter,
	float MaxDelta) const
{
	const FRPGSavedMove_Character* NewMove = static_cast<const FRPGSavedMove_Character*>(NewMovePtr.Get());

	// As an optimization, check if the engine can combine saved moves.
	if (bSavedRequestMovementSettingsChange != NewMove->bSavedRequestMovementSettingsChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMovePtr, InCharacter, MaxDelta);
}

void FRPGSavedMove_Character::Clear()
{
	Super::Clear();

	// Clear all values
	bSavedRequestMovementSettingsChange = 0;
}

uint8 FRPGSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/* There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Unused
	FLAG_Custom_1		= 0x20, // Unused
	FLAG_Custom_2		= 0x40, // Unused
	FLAG_Custom_3		= 0x80, // Unused
	*/

	// Write to the compressed flags 
	if (bSavedRequestMovementSettingsChange)
		Result |= FLAG_Custom_0;
	
	return Result;
}

void FRPGSavedMove_Character::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
	FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UALSCharacterMovementComponent* CharacterMovementComp = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (IsValid(CharacterMovementComp))
	{
		// Copy values into the saved move
		bSavedRequestMovementSettingsChange = CharacterMovementComp->bRequestMovementSettingsChange;
		MaxSpeed = CharacterMovementComp->NewMaxWalkSpeed;
	}
}

void FRPGSavedMove_Character::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UALSCharacterMovementComponent* CharacterMovementComp = Cast<UALSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (IsValid(CharacterMovementComp))
	{
		// Copy values out of the saved move
		CharacterMovementComp->bRequestMovementSettingsChange = bSavedRequestMovementSettingsChange;
	}
}
#pragma endregion FSavedMove_Character

#pragma region FNetworkPredictionData_Client
FRPGNetworkPredictionData_Client_Character::FRPGNetworkPredictionData_Client_Character(
	const UCharacterMovementComponent& ClientMovementComp) : Super(ClientMovementComp)
{
	
}

FSavedMovePtr FRPGNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return MakeShared<FRPGSavedMove_Character>();
}
#pragma endregion FNetworkPredictionData_Client