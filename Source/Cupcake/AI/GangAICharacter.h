// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GangAIManager.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "GangAICharacter.generated.h"

class UAIPerceptionComponent;

UCLASS()
class AGangAICharacter : public ACharacter,  public IDamageableInterface
{
	GENERATED_BODY()

public:
	AGangAICharacter();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser);

	UFUNCTION()
	virtual void OnDeath_Implementation();

	UFUNCTION()
	void OnDamage_Implementation();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartChasing(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void Patrol();

	bool IsChasing() const { return bIsChasing; }

	FVector GetRandomPatrolPoint();

	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere)
	float PatrolRadius;

protected:

	bool bIsChasing;

	//PROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	//UAIPerceptionComponent* PerceptionComponent;

	AGangAIManager* AIManager;
	
};
