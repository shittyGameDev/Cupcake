// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Actor.h"
#include "Tree.generated.h"

class UCapsuleComponent;

UCLASS()
class CUPCAKE_API ATree : public AActor, public IDamageableInterface
{
	GENERATED_BODY()
	
public:	
	ATree();
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
//HEJ
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Mesh")
	UCapsuleComponent* Collider;
public:	
	virtual void Tick(float DeltaTime) override;

};
