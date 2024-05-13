// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cupcake/Items/BaseItem.h"
#include "NewInventoryComponent.generated.h"

class ABaseHUD;
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_MULTICAST_DELEGATE(FOnKeyItemAdded);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnPickup, UBaseItem*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, UBaseItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveItem, UBaseItem*, DropItem);

class UBaseItem;

UENUM()
enum class EItemAddResult : uint8
{
	Iar_NoItemAdded UMETA(DisplayName = "No item added"),
	Iar_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	Iar_AllItemAdded UMETA(DisplayName = "All of the item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : ActualAmountAdded(0),
	OperationResult(EItemAddResult::Iar_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	// Actual amount of item that was added to the inventory (Victor)
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;
	// Enum representing the end state of an add item operation (Victor)
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;
	// Informational message that can be passed with the result (Victor)
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;
	
	// HELPER FUNCTIONS
	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::Iar_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::Iar_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::Iar_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UNewInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	//DELEGATER
	FOnKeyItemAdded OnKeyItemAdded;
	FOnInventoryUpdated OnInventoryUpdated;
	FOnPickup OnPickup;
	FOnRemoveItem OnRemoveItem;
	
	UNewInventoryComponent();

	UFUNCTION(Category= "Inventory")
	FItemAddResult HandleAddItem(UBaseItem* InputItem);

	UFUNCTION(Category= "Inventory")
	UBaseItem* FindMatchingItem(UBaseItem* ItemIn) const;
	UFUNCTION(Category= "Inventory")
	UBaseItem* FindNextItemByID(UBaseItem* ItemIn) const;
	UFUNCTION(Category= "Inventory")
	UBaseItem* FindNextPartialStack(UBaseItem* ItemIn) const;

	UFUNCTION(Category= "Inventory")
	void RemoveSingleInstanceOfItem(UBaseItem* ItemToRemove);
	UFUNCTION(Category= "Inventory")
	int32 RemoveAmountOfItem(UBaseItem* ItemIn, int32 DesiredAmountToRemove);
	UFUNCTION(Category= "Inventory")
	void SplitExistingStack(UBaseItem* ItemIn, const int32 AmountToSplit);
	
	UFUNCTION(Category= "Inventory")
	bool HasItemByID(FName ID);

	//Getters
	// ----------------------
	UFUNCTION(Category= "Inventory")
	FORCEINLINE int GetSlotsCapacity() const { return InventorySlotsCapacity; };
	UFUNCTION(Category= "Inventory")
	FORCEINLINE TArray<UBaseItem*> GetInventoryContents() const { return InventoryContents;};

	//Setters
	// ----------------------
	UFUNCTION(Category= "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };

protected:
	// Called when the game starts

	UPROPERTY(EditInstanceOnly, Category="Inventory")
	int32 InventorySlotsCapacity;

	//Actual Inventory
	UPROPERTY(VisibleAnywhere, Category="Inventory")
	TArray<TObjectPtr<UBaseItem>> InventoryContents;
	
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UBaseItem* InputItem, int32 RequestedAddAmount);
	int32 HandleStackableItems(UBaseItem* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UBaseItem* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UBaseItem* Item, const int32 AmountToAdd);
};
