// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Cupcake/Items/Item.h"
#include "HotbarWidget.generated.h"


class UInventoryComponent;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UHotbarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	// Active item
	AItem* EquippedItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> UIImages;

	UPROPERTY()
	TArray<UBorder*> UIBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTextBlock*> Quantity;

	UFUNCTION(BlueprintCallable)
	void SetQuantity(TArray<UTextBlock*> NewQuantity);

	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere)
	FLinearColor HighlightColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.f); // Dark grey with 60% opacity
	
	UFUNCTION(BlueprintCallable)
	void UpdateHotbar(const TArray<AItem*>& Items);
	
	// Setter function for UIImages
	UFUNCTION(BlueprintCallable)
	void SetUIImages(TArray<UImage*> NewUIImages);

	UFUNCTION(BlueprintCallable)
	void SetUIBorders(TArray<UBorder*> NewUIBorder);

	UFUNCTION()
	void HighlightImage(int32 ImageIndex);

	UFUNCTION()
	void ResetHighlight(int32 ImageIndex);

	UFUNCTION(BlueprintCallable)
	void HighLightUIItem(FKey KeyPressed);

	UPROPERTY()
	TMap<FKey, int32> KeyBindings = {
		{EKeys::One, 0},
		{EKeys::Two, 1},
		{EKeys::Three, 2},
		// Add more key bindings as needed
	};
	
};
