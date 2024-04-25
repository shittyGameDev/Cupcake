// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheMapHandler.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CUPCAKE_API UTheMapHandler : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageOpacity(const FName& ImageTag, float NewOpacity);
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageByTag(const FName& Tag);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Image1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Image2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Image3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Image4;

	TMap<FName, UImage*> Images;
};
