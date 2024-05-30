// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VolumeSave.generated.h"

UCLASS()
class CUPCAKE_API UVolumeSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float MasterVolume;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float SFXVolume;
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	float MusicVolume;
};
