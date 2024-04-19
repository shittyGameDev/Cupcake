// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CupcakeGameMode.h" // Include CupcakeGameMode header
#include "MapTriggerBoxSpecificActor.generated.h"

UCLASS()
class CUPCAKE_API AMapTriggerBoxSpecificActor : public ATriggerBox
{
	GENERATED_BODY()

protected:
	// Called when an actor begins to overlap this trigger box
	virtual void BeginPlay() override;

public:
	// Called when an actor enters the trigger box
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// Called when an actor leaves the trigger box
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	// Boolean variable to store overlap status
	UPROPERTY(EditAnywhere, Category="Map")
	bool bIsOverlapping;

	// Boolean variable to track if the player has already set the overlap flag
	UPROPERTY(EditAnywhere, Category="Map")
	bool bPlayerOverlapped;

	UPROPERTY(EditAnywhere, Category="Map")
	FName TriggerBoxName;
};