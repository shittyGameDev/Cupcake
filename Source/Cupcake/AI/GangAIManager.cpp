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

	for (auto& AIChar : RegisteredAICharacters)
	{
		if (AIChar)
		{
			// Tvingar alla karaktärer att starta eller uppdatera sin jakt
			UE_LOG(LogTemp, Warning, TEXT("AI Character %s is now chasing"), *AIChar->GetName());
			AIChar->StartChasing(UGameplayStatics::GetPlayerPawn(AIChar, 0));
		}
	}
}

void AGangAIManager::RegisterAICharacter(AGangAICharacter* AICharacter)
{
	RegisteredAICharacters.AddUnique(AICharacter);
}
