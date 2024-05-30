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

	bool RegenerateHealth(float HealthAmount);
	
	void DoDamage(float DamageAmount);
	void ReceiveDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

private:
	// Current Health
	UPROPERTY(Blueprintable, EditAnywhere, Category="Actor Attributes")
	float Health = 100.f;
	 
	UPROPERTY(Blueprintable, EditAnywhere, Category="Actor Attributes")
	float MaxHealth = 100.f;
};