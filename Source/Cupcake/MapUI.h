// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Components/Image.h"
#include "MapUI.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKE_API UMapUI : public UUserWidgetBlueprint
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void ShowImagesOnTrigger(const FName& TriggerBoxName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TArray<UImage*> Images;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image;
	
	
};
