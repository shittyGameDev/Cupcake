// AGangAIManager.cpp

#include "GangAIManager.h"
#include "GangAICharacter.h"
#include "Kismet/GameplayStatics.h"

// Constructor
AGangAIManager::AGangAIManager()
{
    // Uncomment and use if you need a BillboardComponent as the root component
    // BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
    // RootComponent = BillboardComponent;
}

// Initiates a group chase for all registered AI characters
void AGangAIManager::InitiateGroupChase()
{
    UE_LOG(LogTemp, Warning, TEXT("Initiating group chase for %d characters"), RegisteredAICharacters.Num());

    // Get the player pawn
    AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player) return;

    // Loop through all registered AI characters
    for (AGangAICharacter* AIChar : RegisteredAICharacters)
    {
        if (AIChar)
        {
            // Start chasing the player
            AIChar->StartChasing(Player);
        }
    }
}

// Registers an AI character with the manager
void AGangAIManager::RegisterAICharacter(AGangAICharacter* AICharacter)
{
    if (AICharacter)
    {
        RegisteredAICharacters.AddUnique(AICharacter);
    }
}

// Determines if the requesting AI can attack (only the closest AI can attack)
bool AGangAIManager::CanAttack(AGangAICharacter* RequestingAI)
{
    if (RegisteredAICharacters.Num() == 0) return false;

    // Get the player pawn
    AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player) return false;

    // Find the closest AI character to the player
    AGangAICharacter* ClosestAI = nullptr;
    float MinDistanceSq = FLT_MAX;

    for (AGangAICharacter* AIChar : RegisteredAICharacters)
    {
        if (AIChar && IsValid(AIChar))
        {
            float DistanceSq = FVector::DistSquared(AIChar->GetActorLocation(), Player->GetActorLocation());
            if (DistanceSq < MinDistanceSq)
            {
                MinDistanceSq = DistanceSq;
                ClosestAI = AIChar;
            }
        }
    }

    // Only the closest AI can attack
    return ClosestAI == RequestingAI;
}
