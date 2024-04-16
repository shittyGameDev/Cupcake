// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Health.generated.h"

class UHealthComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUPCAKE_API IHealth
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UHealthComponent* HealthComponent;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDamage();
};