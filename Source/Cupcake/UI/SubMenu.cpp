// Fill out your copyright notice in the Description page of Project Settings.


#include "SubMenu.h"
#include "Components/Button.h"

void USubMenu::NativeConstruct()
{
	Super::NativeConstruct();
	

	// Bind the button click events to their respective functions
	if (MyButton1)
	{
		MyButton1->OnClicked.AddDynamic(this, &USubMenu::MenuItemClicked);
	}
	if (MyButton2)
	{
		MyButton2->OnClicked.AddDynamic(this, &USubMenu::MenuItemClicked);
	}
}

void USubMenu::MenuItemClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("MenuItemClicked"));
}


