// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

<<<<<<<< HEAD:Source/Cupcake/Actors/HealthComponent.h
	bool RegenerateHealth(float HealthAmount);
	
	void DoDamage(float DamageAmount);
};
========
private:
	// Current Health
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health = 100.f;
	 
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth = 100.f;

public:
	void ReceiveDamage(float Damage);
};
>>>>>>>> main:Source/Cupcake/Actors/AttributeComponent.h
