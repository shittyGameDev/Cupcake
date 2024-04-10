// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
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

	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHotbar(const TArray<AItem*>& Items);
	
};
