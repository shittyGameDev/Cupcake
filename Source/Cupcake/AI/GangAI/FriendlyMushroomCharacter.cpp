// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendlyMushroomCharacter.h"

// Sets default values
AFriendlyMushroomCharacter::AFriendlyMushroomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFriendlyMushroomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AFriendlyMushroomCharacter::MoveToLocation(const FVector& NewLocation)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(NewLocation);
	}
}

