// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UInventoryTooltip;
class UDragItemVisual;
class UBaseItem;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UBaseItem* ItemIn){ ItemReference = ItemIn; }
	FORCEINLINE UBaseItem* GetItemReference() const { return ItemReference;}

protected:

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UBaseItem* ItemReference;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UTextBlock* ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
