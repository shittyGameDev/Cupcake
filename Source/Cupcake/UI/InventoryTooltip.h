// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Cupcake/Items/BaseItem.h"
#include "InventoryTooltip.generated.h"

class UTextBlock;
class UInventoryItemSlot;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateTooltip(UBaseItem* Item);
	
	UPROPERTY(VisibleAnywhere)
	UInventoryItemSlot* InventorySlotBeingHovered;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemType;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* UsageText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* StackSizeText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxStackSizeText;

protected:
	virtual void NativeConstruct() override;

};
