// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemSlot.h"
#include "SubMenu.h"

#include "DragItemVisual.h"
#include "InventoryTooltip.h"
#include "ItemDragDropOperation.h"
#include "Components/Image.h"
#include "Cupcake/Actors/AttributeComponent.h"
#include "Cupcake/Items/BaseItem.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"

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

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightMouseButtonClick(InGeometry, InMouseEvent);
		return FReply::Handled();
	}

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


//Denna metod ska reworkas i och med att vi kollar på ItemDataStructs. Allt man ser här är EXTREMT TILLFÄLLIGT i mån om tid. (Victor)
void UInventoryItemSlot::OnRightMouseButtonClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(ItemReference)
	{
		if(ItemReference->ID.IsEqual("test_001"))
		{
			PlayerCharacter = Cast<ACupcakeCharacter>(ItemReference->OwningInventory->GetOwner());
			if(PlayerCharacter->Attributes->RegenerateHealth(10.f))
			{
				PlayerCharacter->RemoveItemFromInventory(ItemReference, 1);
				UE_LOG(LogTemp, Warning, TEXT("New Health: %f"), PlayerCharacter->Attributes);
				UE_LOG(LogTemp, Warning, TEXT("Used berry"));
			}
			
		}
	}
}

