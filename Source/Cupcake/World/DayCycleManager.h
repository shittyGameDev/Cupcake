// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/Actor.h"
#include "Engine/SkyLight.h"
#include "DayCycleManager.generated.h"


DECLARE_DYNAMIC_DELEGATE(FTimeSpecificEvent);

USTRUCT()
struct FTimeEvent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int Day;

	UPROPERTY()
	int Hour;

	UPROPERTY()
	int Minute;

	FTimeSpecificEvent EventDelegate;
};


UCLASS()
class CUPCAKE_API ADayCycleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayCycleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMesh* CubeMesh;
	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere)
	FRotator SpawnRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	ASkyLight* SkyLight;

	UPROPERTY(EditAnywhere)
	ADirectionalLight* DirectionalLight;
	// Get methods
	int GetCurrentDayNumber();
	int GetHour();
	int GetMinutes();

	// Functions
	void ShiftTime(float Time);

	void RegisterTimeEvent(const FTimeEvent& NewEvent);
	UFUNCTION()
	void SpawnTreeEvent();

private:
	char DayCycle = 0;
	float ElapsedTime = 0.f;
	float const SECONDS_IN_A_DAY = 100.f;

	TArray<FTimeEvent> TimeEvents;
};