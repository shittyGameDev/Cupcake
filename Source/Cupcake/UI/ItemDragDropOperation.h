// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UNewInventoryComponent;
class UBaseItem;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	UBaseItem* SourceItem;

	UPROPERTY()
	UNewInventoryComponent* SourceInventory;
};
