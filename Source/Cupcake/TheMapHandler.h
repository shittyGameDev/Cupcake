// TheMapHandler.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TheMapHandler.generated.h"

UCLASS()
class CUPCAKE_API UTheMapHandler : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Image1;
	UPROPERTY(meta = (BindWidget))
	UImage* Image2;
	UPROPERTY(meta = (BindWidget))
	UImage* Image3;
	UPROPERTY(meta = (BindWidget))
	UImage* Image4;

	// En metod för att dölja en specifik bild
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideImage(int32 ImageIndex);
};