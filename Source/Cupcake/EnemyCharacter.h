// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Actors\DamagableComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class CUPCAKE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	//virtual void OnDeath_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	AWeaponBase* WeaponComponent;

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, Category="Health")
	float Health;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
};
