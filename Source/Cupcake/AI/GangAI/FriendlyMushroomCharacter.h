#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "FriendlyMushroomCharacter.generated.h"

UCLASS()
class CUPCAKE_API AFriendlyMushroomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFriendlyMushroomCharacter(); 

	// Metod för att flytta NPC till specifik plats
	void MoveToLocation(const FVector& NewLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector Destination;
};
