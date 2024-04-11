// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Actors/Combat.h"
#include "Actors/Health.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class CUPCAKE_API AEnemyCharacter : public ACharacter, public ICombat, public IHealth
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UHealthComponent* HealthComponent;
	virtual void OnDeath_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	AWeaponBase* WeaponComponent;

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, Category="Health")
	float Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
