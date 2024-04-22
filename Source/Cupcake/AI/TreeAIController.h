// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h" // Include PlayerController header
#include "TreeAIController.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKE_API ATreeAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override; // Override BeginPlay function

	virtual void Tick(float DeltaTime) override;

private:
	APawn* PlayerPawn; // Pointer to PlayerController

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};