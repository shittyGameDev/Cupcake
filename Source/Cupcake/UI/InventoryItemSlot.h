// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class ACupcakeCharacter;
class USubMenu;
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
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHoverAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHoverLeaveAnim();
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot",meta=(BindWidget))
	UImage* ItemIcon;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEatSound();
protected:

	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<USubMenu> SubMenuClass;
	
	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UBaseItem* ItemReference;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot", meta=(BindWidget))
	UBorder* ItemBorder;



	UPROPERTY(VisibleAnywhere, Category="Inventory Slot",meta=(BindWidget))
	UTextBlock* ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	void OnRightMouseButtonClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
