// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayCycleManager.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API ADayCycleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayCycleManager();
    
    // Array of event list items
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Events")
    //TArray<FEventListItem> EventList;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USkyLightComponent* SkyLightComponent;

	// Get methods
	int GetCurrentDayNumber();
	int GetHour();
	int GetMinutes();

	// Functions
	void ShiftTime(float Time);
	

private:
	char DayCycle = 0;
	float ElapsedTime = 0;
	float const SECONDS_IN_A_DAY = 84000.f; 
	
};

USTRUCT(BlueprintType)
struct FEventListItem
{
	GENERATED_BODY()

	// Pointer to the UEvent object
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	//UEvent* Event;

	// Hour (0-23)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int Hour;

	// Minutes (0-59)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	int Minutes;

	// Default constructor
	//FEventListItem() : Event(nullptr), Hour(0), Minutes(0) {}

	// Constructor with parameters
	//FEventListItem(UEvent* InEvent, int InHour, int InMinutes) : Event(InEvent), Hour(InHour), Minutes(InMinutes) {}
};