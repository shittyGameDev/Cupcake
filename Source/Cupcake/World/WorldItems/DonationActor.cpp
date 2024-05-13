// Fill out your copyright notice in the Description page of Project Settings.


#include "DonationActor.h"

#include "ObeliskActor.h"

// Sets default values
ADonationActor::ADonationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void ADonationActor::BeginPlay()
{
	Super::BeginPlay();

	bIsMoving = false;

	if(ObeliskActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DESTROYING ACTOR"));
		ObeliskActor->OnDonationGoalReached.AddUObject(this, &ADonationActor::DonationGoalAction);
	}

	UE_LOG(LogTemp, Warning, TEXT("Obelisk Actor: %p"), ObeliskActor);
}

// Called every frame
void ADonationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle the movement over time
	if (bIsMoving)
	{
		if (MoveTimer < MoveDuration)
		{
			// Increment the timer
			MoveTimer += DeltaTime;

			// Interpolate the current location to the target location
			FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, MoveTimer / MoveDuration);
			SetActorLocation(NewLocation);

			// Check if the move is complete
			if (MoveTimer >= MoveDuration)
			{
				bIsMoving = false;
				MoveTimer = 0.0f;  // Reset the timer
			}
		}
	}

}

void ADonationActor::DonationGoalAction()
{
	if (!bIsMoving)  // Ensure that we are not already moving
		{
		TargetLocation = GetActorLocation() + FVector(0, 0, MoveDistance);
		MoveTimer = 0.0f;  // Reset the timer
		bIsMoving = true;  // Start moving
		}
}


