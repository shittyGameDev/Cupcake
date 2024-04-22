// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingWidget.generated.h"

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
	
	UPROPERTY(meta=(BindWidget))
	UWrapBox* CraftingPanel;

	UPROPERTY(meta=(BindWidget))
	UImage* CraftingICon;

	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;

	UPROPERTY()
	UNewInventoryComponent* InventoryReference;
	
};
