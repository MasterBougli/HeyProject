// Copyright 2020-2030 AlemdarLabs, All Rights Reserved

#include "Player/ALSPlayerController.h"

AALSPlayerController::AALSPlayerController()
{
	
}

void AALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	OnPawnReplicated(GetPawn());
}
