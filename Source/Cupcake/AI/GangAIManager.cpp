// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAIManager.h"

#include "GangAICharacter.h"
#include "Kismet/GameplayStatics.h"


void UGangAIManager::InitiateGroupChase()
{
	for (auto& AIChar : RegisteredAICharacters)
	{
		if (AIChar && !AIChar->IsChasing())
		{
			AIChar->StartChasing(UGameplayStatics::GetPlayerPawn(AIChar, 0));
		}
	}
}

void UGangAIManager::RegisterAICharacter(AGangAICharacter* AICharacter)
{
	RegisteredAICharacters.AddUnique(AICharacter);
}
