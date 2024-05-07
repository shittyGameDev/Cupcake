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
	// Sets default values for this actor's properties
	ATree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Mesh")
	UStaticMeshComponent* Mesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
