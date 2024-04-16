// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Data/ItemDataStructs.h"
#include "BaseItem.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKE_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	//UInventoryComponent* OwningInventory;
	//UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin=1, UIMax=64))
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, Category= "Item ID")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category= "Item Type")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category= "Item Text Data")
	FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category= "Item Numeric Data")
	FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category= "Item Asset Data")
	FItemAssetData AssetData;

	UBaseItem();

	UBaseItem* CreateItemCopy() const;

	UFUNCTION(Category="Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize;};

	UFUNCTION(Category="Item")
	void SetQuantity(const int32 NewQuantity);
	UFUNCTION(Category="Item")
	virtual void Use(ACupcakeCharacter* Character);
protected:
	bool operator==(const FName& OtherID) const
	{
		return ID == OtherID;
	} 
};
