// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropComponent.generated.h"


class ACupcakeCharacter;
class UBaseItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDropComponent();

	void InitializeDrop(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	//--------------------------------------------------------------------------------
	//------------VARIABLES----------------------------------------------------------
	
	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	UDataTable* ItemDataTable;
	
	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Drop | Item Reference")
	UBaseItem* ItemReference;

	//--------------------------------------------------------------------------------
	//------------Functions----------------------------------------------------------

	UFUNCTION(BlueprintCallable)
	void DropItem(ACupcakeCharacter* PlayerCharacter) const;	
};
