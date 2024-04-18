// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NewItem.generated.h"

class UImage;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNewItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUPCAKE_API INewItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
private:
	FString ID;
	FString Description;
	UImage* Image;
	
public:
	FString GetID();
	UImage* GetImage();
	
	void Use();
};
