// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "CraftingBenchActor.generated.h"

class UBaseItem;
class UNewInventoryComponent;
class UCraftingWidget;

UCLASS()
class CUPCAKE_API ACraftingBenchActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingBenchActor();
	
	void InitializeCraftedItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);
	void InitializeCraftedItemCost(TSubclassOf<UBaseItem> BaseClass, int32 InQuantity);

	UPROPERTY(EditAnywhere, Category="Crafting values")
	int32 CraftingCost;

	UPROPERTY(EditAnywhere, Category="Crafting values")
	FName RecipeItemID;

	UPROPERTY(VisibleAnywhere, Category="Crafting Reference")
	UBaseItem* RecipeItem;

	UPROPERTY(EditAnywhere, Category="Crafting values")
	int32 CraftedAmount;

protected:
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
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;
	bool IsValidItemAndRecipe() const;
	void HandleItemRemovalAndAddition(ACupcakeCharacter* PlayerCharacter, UNewInventoryComponent* PlayerInventory);
	void HandleAddResult(const FItemAddResult& AddResult, ACupcakeCharacter* PlayerCharacter) const;
};
