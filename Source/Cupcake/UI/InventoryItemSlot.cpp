// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemSlot.h"
#include "SubMenu.h"

#include "DragItemVisual.h"
#include "InventoryTooltip.h"
#include "ItemDragDropOperation.h"
#include "Components/Image.h"
#include "Cupcake/Items/BaseItem.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(ToolTipClass)
	{
		UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, ToolTipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemReference)
	{

		//HÄR KOMMER DET VARA KOD FÖR ATT ÄNDRA ITEMBORDER FÄRG BEROENDE PÅ ITEM

		
		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if(ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	/*else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightMouseButtonClick(InGeometry, InMouseEvent);
		return FReply::Handled();
	}*/

	// submenu on right click will happen here

	return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if(DragItemVisualClass)
	{
		const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

/*void UInventoryItemSlot::OnRightMouseButtonClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Create the submenu widget (assuming UMySubMenu is a UUserWidget)
	USubMenu* SubMenu = CreateWidget<USubMenu>(this, SubMenuClass);
    
	if (SubMenu)
	{
		// Set the position of the submenu
		FVector2D MenuPosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		SubMenu->SetPositionInViewport(MenuPosition);

		
		if (!SubMenu->IsInViewport())
		{
			SubMenu->AddToViewport();
		}
	}
}*/

