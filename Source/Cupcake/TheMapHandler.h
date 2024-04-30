// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TheMapHandler.generated.h"

/**
 * Singleton class to handle map images.
 */
UCLASS()
class CUPCAKE_API UTheMapHandler : public UUserWidget
{
	GENERATED_BODY()

public:
	static UTheMapHandler* GetInstance();

	void printImage1();
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageOpacity(const FName& ImageTag, float NewOpacity);
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageByTag(const FName& Tag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* Image1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* Image2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* Image3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* Image4;
	UPROPERTY()
	TMap<FName, UImage*> Images;

protected:
	UTheMapHandler(const FObjectInitializer& ObjectInitializer);




private:
	static UTheMapHandler* Instance;
};