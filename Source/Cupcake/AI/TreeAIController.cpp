// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ATreeAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	}
}

void ATreeAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	if (LineOfSightTo(PlayerPawn))
	{
		SetFocus(PlayerPawn);
		MoveToActor(PlayerPawn, AcceptanceRadius);
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
	*/
}

/*
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
*/