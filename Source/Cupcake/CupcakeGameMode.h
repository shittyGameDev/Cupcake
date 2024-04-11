// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CupcakeGameMode.generated.h"

UCLASS()
class CUPCAKE_API ACupcakeGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<FName> TriggerBoxNames;

	UPROPERTY(EditAnywhere)
	TArray<bool> TriggerBoxBoolArray;

public:
	UFUNCTION(BlueprintCallable)
	void AddTriggerBox(bool bBoolValue, const FName& BoxName);

	UFUNCTION(BlueprintCallable)
	bool GetTriggerBoxBool(const FName& BoxName) const;

	UFUNCTION(BlueprintCallable)
	void LogTriggerBoxes() const;
	
	virtual void BeginPlay() override;
};
