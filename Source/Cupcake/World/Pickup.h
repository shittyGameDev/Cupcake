// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UTimelineComponent;
struct FTimeline;
class UBaseItem;
class UNiagaraSystem;

UCLASS()
class CUPCAKE_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APickup();

	void InitializePickup(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);

	void InitializeDrop(UBaseItem* ItemToDrop, const int32 InQuantity);

	UFUNCTION()
	void StartScaling(UCurveFloat* ScaleCurve);

	UFUNCTION()
	void HandleScaling(float Value);

	UFUNCTION()
	void FinishScaling() const;
	
	FORCEINLINE UBaseItem* GetItemData(){ return ItemReference;}

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	UPROPERTY()
	UTimelineComponent* Timeline;

	UPROPERTY(VisibleAnywhere, Category= "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, Category= "Pickup | Item Initialization")
	UDataTable* ItemDataTable;
	
	UPROPERTY(EditAnywhere, Category= "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category= "Pickup | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Pickup | Item Reference")
	UBaseItem* ItemReference;
	
	UPROPERTY(VisibleInstanceOnly, Category= "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;

	void UpdateInteractableData();
	
	void TakePickup(const ACupcakeCharacter* Taker);
	
};
