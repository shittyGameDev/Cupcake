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

// Called every frame
void AFriendlyMushroomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFriendlyMushroomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

