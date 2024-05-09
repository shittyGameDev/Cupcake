// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RepairWidget.generated.h"

class UTextBlock;
class UImage;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class CUPCAKE_API URepairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* StoneImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StoneQuantity;
	UPROPERTY(Transient,BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* StoneAnim;
	
	UPROPERTY(meta=(BindWidget))
	UImage* IronImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* IronQuantity;
	UPROPERTY(Transient,BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* IronAnim;
	
	UPROPERTY(meta=(BindWidget))
	UImage* WoodImage;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WoodQuantity;
	UPROPERTY(Transient,BlueprintReadOnly, meta = (BindWidgetAnim))
	UWidgetAnimation* WoodAnim;


	void IncreaseWoodQuantity(int NewWoodQuantity);
	void IncreaseStoneQuantity(int NewWoodQuantity);
	void IncreaseIronQuantity(int NewWoodQuantity);
	
};
