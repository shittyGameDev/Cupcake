// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ObeliskActor.generated.h"

class UBaseItem;
class UNewInventoryComponent;

UCLASS()
class CUPCAKE_API AObeliskActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObeliskActor();

	void InitializeObeliskItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category="Test Actor")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	UDataTable* ItemDataTable;
	
	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Craft | Item Reference")
	UBaseItem* ItemReference;

	UPROPERTY()
	UNewInventoryComponent* InventoryReference;

	UPROPERTY(VisibleAnywhere)
	int32 NumberOfItemsDonated;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;
};
