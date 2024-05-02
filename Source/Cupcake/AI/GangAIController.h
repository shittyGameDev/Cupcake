// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GangAIController.generated.h"


UCLASS()
class CUPCAKE_API AGangAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGangAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
};
