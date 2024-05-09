// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAIManager.h"

#include "GangAICharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


AGangAIManager::AGangAIManager()
{
	//BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	//RootComponent = BillboardComponent;
}

void AGangAIManager::InitiateGroupChase()
{
	UE_LOG(LogTemp, Warning, TEXT("Initiating group chase for %d characters"), RegisteredAICharacters.Num());
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player) return;

	for (auto& AIChar : RegisteredAICharacters)
	{
		if (AIChar)
		{
			AIChar->StartChasing(Player); // All AIs will start chasing the player
			
		}
	}
}

void AGangAIManager::RegisterAICharacter(AGangAICharacter* AICharacter)
{
	RegisteredAICharacters.AddUnique(AICharacter);
}

bool AGangAIManager::CanAttack(AGangAICharacter* RequestingAI)
{
	if (!RegisteredAICharacters.Num()) return false;

	// Sort AI characters by their distance to the player
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if(!Player)
	{
		return false;
	}
	RegisteredAICharacters.Sort([Player](const AGangAICharacter& A, const AGangAICharacter& B) {
		return FVector::Dist(A.GetActorLocation(), Player->GetActorLocation()) <
			   FVector::Dist(B.GetActorLocation(), Player->GetActorLocation());
	});

	return RegisteredAICharacters[0] == RequestingAI; // Only the closest AI can attack
}
