// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cupcake/Items/Item.h"
#include "Cupcake/Items/NewItem.h"
#include "Cupcake/UI/HotbarWidget.h"
#include "InventoryComponent.generated.h"

struct InventorySlot
{
	INewItem* Item;
	int Quantity;

	FString ItemName()
	{
		if (Item != nullptr)
			{
				return Item->GetID();
			}
		return FString(TEXT("No Item"));
	}

	UImage* ItemImage()
	{
		if (Item != nullptr)
			{
				return Item->GetImage();
			}
		return nullptr;
	}

	void AddItem(INewItem* NewItem, int NumToAdd)
	{
		if (Item == nullptr)
		{
			Item = NewItem;
		}
		Quantity += NumToAdd;
	}

	void RemoveItem()
	{
		if (Quantity == 0)
		{
			Item == nullptr;
		}
		else if (Quantity > 0)
		{
			Quantity--;
		}
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY()
	TArray<AItem*> InventoryItems;

	UFUNCTION()
	void AddItem(AItem* Item);

	UFUNCTION()
	void RemoveItem(AItem* Item);

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HotbarWidgetClass;

	UPROPERTY()
	UHotbarWidget* HotbarWidget;

private:
	UFUNCTION()
	AItem* FindItemByName(FString ItemName);

	UFUNCTION()
	void LogInventory();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InventoryHUDClass;
	

	UPROPERTY()
	UUserWidget* InventoryHUD;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
