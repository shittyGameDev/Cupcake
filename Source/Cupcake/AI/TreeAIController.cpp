// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeAIController.h"

void ATreeAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController(); // Get pointer to the first player controller
	LookAtPlayer(); // Call LookAtPlayer function
}

void ATreeAIController::LookAtPlayer()
{
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // Get the player's pawn
		if (PlayerPawn)
		{
			SetFocus(PlayerPawn); // Set the player's pawn as the focus of the AI
		}
	}
}