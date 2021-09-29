// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Libraries/ALSStructLibrary.h"
#include "ALSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class REPLICATEDALS_API UALSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

friend class FRPGSavedMove_Character;

public:
	UALSCharacterMovementComponent();
	virtual void BeginPlay() override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable, Category = "ALS | Components")
	bool IsCustomMovementMode(uint8 CustomMovementModeValue) const;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void PhysWalking(float DeltaTime, int32 Iterations) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	UPROPERTY()
	class AALSCharacterBase* PlayerOwner;
	
private:
	// For Advanced Locomotion System
	uint8 bRequestMovementSettingsChange : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS | Movement Settings", Meta = (AllowPrivateAccess))
	float NewMaxWalkSpeed = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS | Movement Settings", Meta = (AllowPrivateAccess))
	FALSMovementSettings CurrentMovementSettings;

public:
	// Set Movement Curve (Called in every instance)
	UFUNCTION(BlueprintCallable, Category = "ALS | Movement Settings")
	float GetMappedSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "ALS | Movement Settings")
	void SetMovementSettings(FALSMovementSettings NewMovementSettings);

	UFUNCTION(BlueprintCallable, Category = "ALS | Movement Settings")
	float GetSpeedForGait(const EALSGait Gait) const;
	
	// Set Max Walking Speed (Called from the owning client)
	UFUNCTION(BlueprintCallable, Category = "ALS | Movement Settings")
	void SetMaxWalkingSpeed(float UpdateMaxWalkSpeed);

	UFUNCTION(Reliable, Server, Category = "ALS | Movement Settings")
	void Server_SetMaxWalkingSpeed(float UpdateMaxWalkSpeed);
	
};

#pragma region FSavedMove_Character
class FRPGSavedMove_Character : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;

	FRPGSavedMove_Character();

	/* *
	 * This is used to check whether or not two moves can be combined into one.
	 * Basically you just check to make sure that the saved variables are the same.
	 */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, 
		ACharacter* InCharacter, float MaxDelta) const override;

	// Resets all saved variables.
	virtual void Clear() override;

	// Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;

	// Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, 
		FVector const& NewAccel, 
		FNetworkPredictionData_Client_Character& ClientData) override;

	// Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(ACharacter* Character) override;

private:
	// For Advanced Locomotion System
	uint8 bSavedRequestMovementSettingsChange : 1;
	
};
#pragma endregion FSavedMove_Character

#pragma region FNetworkPredictionData_Client
class FRPGNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	FRPGNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);

	//brief Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};
#pragma endregion FNetworkPredictionData_Client
