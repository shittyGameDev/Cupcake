// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialWall.generated.h"

class UNewInventoryComponent;
class ACupcakeCharacter;
class UBoxComponent;

UCLASS()
class CUPCAKE_API ATutorialWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialWall();

	UPROPERTY(EditAnywhere)
	ACupcakeCharacter* PlayerCharacter;

	UPROPERTY()
	UNewInventoryComponent* InventoryComponent;

	UPROPERTY()
	bool bIsTurnedOff = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Collider")
	UBoxComponent* BoxComponent;

	void TurnOffTutWall() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
