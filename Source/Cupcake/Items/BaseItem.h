// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Data/ItemDataStructs.h"
#include "BaseItem.generated.h"

class UNewInventoryComponent;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UNewInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Item ID")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, Category= "Item Type")
	EItemType ItemType;
	
	UPROPERTY(VisibleAnywhere, Category= "Item Text")
	FItemTextData TextData;
	
	UPROPERTY(VisibleAnywhere, Category= "Item Numeric")
	FItemNumericData NumericData;
	
	UPROPERTY(EditAnywhere, Category= "Item Asset")
	FItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;

	
	UBaseItem();

	void ResetItemFlags();

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
		return this->ID == OtherID;
	} 
};
