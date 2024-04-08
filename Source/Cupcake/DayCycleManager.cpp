// Fill out your copyright notice in the Description page of Project Settings.


#include "DayCycleManager.h"

// Sets default values
ADayCycleManager::ADayCycleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADayCycleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADayCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (ElapsedTime >= SECONDS_IN_A_DAY)
	{
		DayCycle++;
		ElapsedTime = 0;
	}
}

int ADayCycleManager::GetCurrentDayNumber()
{
	return DayCycle;
}

int ADayCycleManager::GetHour()
{
	return ElapsedTime / 60 / 60;
}

int ADayCycleManager::GetMinutes()
{
	return static_cast<int>(ElapsedTime) % 60;
}

void ADayCycleManager::ShiftTime(float Time)
{
	ElapsedTime += Time;
	
	int daysPassed = static_cast<int>(ElapsedTime / SECONDS_IN_A_DAY);
	
	DayCycle += daysPassed;
	
	ElapsedTime -= daysPassed * SECONDS_IN_A_DAY;
}

void ADayCycleManager::SetDayAndTime(int Day, int Hour, int Minute)
{
	// Example implementation - adjust your logic as needed
	DayCycle = Day;
	// Convert hours and minutes to seconds and set ElapsedTime accordingly
	ElapsedTime = Hour * 3600.f + Minute * 60.f;

	// Call the Blueprint-implementable event
	OnDayAndTimeSet(Day, Hour, Minute);
}