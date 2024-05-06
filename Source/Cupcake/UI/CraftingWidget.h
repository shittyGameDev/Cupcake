// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

class UBorder;
struct FInteractableData;
class UImage;
class UNewInventoryComponent;
class ACupcakeCharacter;
class UTextBlock;
class UWrapBox;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UCraftingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UBorder* CraftingBorder;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* CraftedItemIcon;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* CraftingItemReq;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* CraftingDescription;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* CraftingQuantity;

	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;

	UPROPERTY()
	UNewInventoryComponent* InventoryReference;
	
};
