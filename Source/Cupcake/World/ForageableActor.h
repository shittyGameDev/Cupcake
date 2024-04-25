// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ForageableActor.generated.h"

class UImage;
class UNiagaraSystem;
class UBaseItem;
class ACupcakeCharacter;
class UNiagaraComponent;


UCLASS()
class CUPCAKE_API AForageableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForageableActor();

	void InitializeForageItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = Curve)
	UCurveFloat* Curve;

	// Function to handle the completion of the timer
	void HandleTimerFinished();

	// Function to start the timer when the player interacts with the actor
	void StartForagingTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;

	

	FTransform CalculateSpawnPoint();

	UPROPERTY(EditAnywhere, Category="Interaction Data")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditAnywhere, Category="Forage Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category= "Forage | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category= "Forage | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category= "Forage | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Forage | Item Reference")
	UBaseItem* ItemReference;

	UPROPERTY(EditAnywhere, Category= "Forage | Visual")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY()
	bool bIsForageable;

	UPROPERTY()
	FTimerHandle TimerHandle;
	

};
