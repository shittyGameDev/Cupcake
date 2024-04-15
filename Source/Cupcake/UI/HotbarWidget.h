// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> UIImages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTextBlock*> Quantity;

	UFUNCTION(BlueprintCallable)
	void SetQuantity(TArray<UTextBlock*> NewQuantity);

	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHotbar(const TArray<AItem*>& Items);
	
	// Setter function for UIImages
	UFUNCTION(BlueprintCallable)
	void SetUIImages(TArray<UImage*> NewUIImages);

	UFUNCTION(BlueprintCallable)
	void HighLightUIItem(FKey KeyPressed);
	
};
