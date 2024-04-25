// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamagableComponent.generated.h"

class UAttributeComponent;

UCLASS(Abstract, Blueprintable)
class CUPCAKE_API UDamagableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDamagableComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UAttributeComponent* AttributeComponent;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDamage();
};