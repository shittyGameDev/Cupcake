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

	UMapUI(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> Images;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* Image;
	
};
