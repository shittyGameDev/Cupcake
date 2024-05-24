// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

class UNiagaraComponent;
class UCapsuleComponent;

UCLASS()
class CUPCAKE_API ATree : public AActor, public IDamageableInterface
{
	GENERATED_BODY()
	
public:	
	ATree();
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Function to trigger the shake
	UFUNCTION()
	void ShakeTree();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Mesh")
	UCapsuleComponent* Collider;

	FVector OriginalPosition;

	UPROPERTY(EditAnywhere, Category = "Shake")
	float ShakeDuration;


	UPROPERTY(EditAnywhere, Category = "Shake")
	float ShakeMagnitude;


	UPROPERTY()
	float ShakeTimeRemaining;
	
	UPROPERTY()
	bool bIsShaking;

	UPROPERTY(EditAnywhere, Blueprintable, Category= "Hit | Visual")
	UNiagaraComponent* NiagaraComponent;


public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLeafParticle();
};
