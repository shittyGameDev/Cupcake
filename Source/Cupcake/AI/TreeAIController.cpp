// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeAIController.h"

#include "Kismet/GameplayStatics.h"

void ATreeAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	LookAtPlayer();

	
	MoveToActor(PlayerPawn);
}

void ATreeAIController::LookAtPlayer()
{
	if (PlayerPawn)
	{
		if (PlayerPawn)
		{
			SetFocus(PlayerPawn); // Set the player's pawn as the focus of the AI
		}
	}
}