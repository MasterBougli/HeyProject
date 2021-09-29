// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALSCharacterBase.generated.h"

UCLASS()
class REPLICATEDALS_API AALSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AALSCharacterBase(const class FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	
	UFUNCTION(BlueprintCallable, Category = "Character | Movement")
	virtual class UALSCharacterMovementComponent* GetCharacterMovementComponent() const;

protected:
	/* Custom movement component*/
	UPROPERTY()
	UALSCharacterMovementComponent* ALSCharacterMovementComponent;
	
};
