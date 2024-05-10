// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "ObeliskActor.generated.h"

class UEndScreen;
class URepairWidget;
class UNiagaraComponent;
class UBaseItem;
class UNewInventoryComponent;


DECLARE_MULTICAST_DELEGATE(FOnDonationGoalReached);

UCLASS()
class CUPCAKE_API AObeliskActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	FOnDonationGoalReached OnDonationGoalReached;
	
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

	UPROPERTY(EditInstanceOnly, Category= "Iron | Item Initialization")
	FName RepairItemID;

	UPROPERTY(EditInstanceOnly, Category= "Iron | Item Initialization")
	int32 RepairItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Iron | Item Reference")
	UBaseItem* RepairItemReference;

	UPROPERTY(EditInstanceOnly, Category= "Stone | Item Initialization")
	FName RepairingDesiredItemID;

	UPROPERTY(EditInstanceOnly, Category= "Stone | Item Initialization")
	int32 RepairingItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Stone | Item Reference")
	UBaseItem* RepairingItemReference;

	UPROPERTY()
	UNewInventoryComponent* InventoryReference;

	UPROPERTY(EditInstanceOnly , Category="Donation")
	int32 DonationGoal = 1;
	
	UPROPERTY(VisibleAnywhere, Category="Donation")
	int32 NumberOfWoodItemsDonated;

	UPROPERTY(VisibleAnywhere, Category="Donation")
	int32 NumberOfStoneItemsDonated;

	UPROPERTY(VisibleAnywhere, Category="Donation")
	int32 NumberOfIronItemsDonated;

	UPROPERTY(EditAnywhere, Category= "Forage | Visual")
	UNiagaraComponent* NiagaraComponent;


	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<URepairWidget> RepairWidgetClass;

	UPROPERTY()
	URepairWidget* RepairWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UEndScreen> EndScreenClass;

	UPROPERTY(EditAnywhere)
	UEndScreen* EndScreen;

	
	
	
	UFUNCTION(BlueprintCallable ,Category="Donation")
	bool CheckIfDonationReached();


public:	
	// Called every frame
	UPROPERTY(VisibleAnywhere)
	bool DonationGoalReached = false;
	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;
};
