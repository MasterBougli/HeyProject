// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#include "Characters/ALSCharacterBase.h"
#include "Components/ALSCharacterMovementComponent.h"

AALSCharacterBase::AALSCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UALSCharacterMovementComponent>(CharacterMovementComponentName))
{
	
}

void AALSCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ALSCharacterMovementComponent = Cast<UALSCharacterMovementComponent>(Super::GetMovementComponent());
}

UALSCharacterMovementComponent* AALSCharacterBase::GetCharacterMovementComponent() const
{
	return ALSCharacterMovementComponent;
}
