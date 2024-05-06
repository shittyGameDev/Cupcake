// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubMenu.generated.h"


class UButton;

UCLASS()
class CUPCAKE_API USubMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void MenuItemClicked();


protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MyButton1;

	UPROPERTY(meta = (BindWidget))
	UButton* MyButton2;

	virtual void NativeConstruct() override;
	
};
