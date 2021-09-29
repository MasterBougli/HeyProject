// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ALSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REPLICATEDALS_API AALSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AALSPlayerController();
	virtual void OnRep_Pawn() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "ALS | Player Controller")
	void OnPawnReplicated(APawn* PossesedPawn);
	
};
