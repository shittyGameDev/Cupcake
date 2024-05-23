// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class ACupcakeCharacter;
class UBaseItem;
class UPickupWidget;
struct FInteractableData;
class UInteractionWidget;
class UMainMenu;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnMenuOpen);

UCLASS()
class CUPCAKE_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	FOnMenuOpen OpenMenu;
	//Håller bara den visuella komponent av denna widget i editorn
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	//Håller bara den visuella komponent av denna widget i editorn
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UPickupWidget> PickupWidgetClass;
	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;

	bool bIsMenuVisible;

	ABaseHUD();

	UFUNCTION(BlueprintCallable)
	void DisplayMenu();
	
	UFUNCTION(BlueprintCallable)
	void HideMenu();
	
	UFUNCTION(BlueprintImplementableEvent)
	void AnimateOpenMenu();
	UFUNCTION(BlueprintImplementableEvent)
	void AnimateCloseMenu();

	UFUNCTION(BlueprintImplementableEvent)
	void AnimateCloseInteractionWidget();
	UFUNCTION(BlueprintImplementableEvent)
	void AnimateOpenInteractionWidget();
	
	void ToggleMenu();

	UFUNCTION()
	void DisplayPickup(UBaseItem* ItemRef);
	UFUNCTION()
	void DisplayDrop(UBaseItem* ItemRef, int32 DropAmount);

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	void UpdateInteractionProgress(float Progress) const;

	UPROPERTY(BlueprintReadWrite)
	UMainMenu* MainMenuWidget;
	UPROPERTY(BlueprintReadWrite)
	UInteractionWidget* InteractionWidget;
	UPROPERTY(BlueprintReadWrite)
	UPickupWidget* PickupWidget;

protected:
	
	
	virtual void BeginPlay() override;
};
