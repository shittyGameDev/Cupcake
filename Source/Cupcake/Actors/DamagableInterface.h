// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamagableInterface.generated.h"

class UHealthComponent;

UINTERFACE(MinimalAPI)
class UHealth : public UInterface
{
	GENERATED_BODY()
};

class CUPCAKE_API IHealth
{
	GENERATED_BODY()
public:
	UHealthComponent* HealthComponent;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDamage();
};