// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupWidget.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UPickupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* PlusMinus;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemQuantity;

	
	
};