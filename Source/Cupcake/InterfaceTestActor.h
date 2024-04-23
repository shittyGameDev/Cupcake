// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

class UBaseItem;
class UNewInventoryComponent;
class UCraftingWidget;

UCLASS()
class CUPCAKE_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterfaceTestActor();
	
	void InitializeCraftedItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);


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
	

	//UPROPERTY(EditAnywhere)
	//UCraftingWidget* CraftingWidget;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;
};
