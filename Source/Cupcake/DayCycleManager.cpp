// Fill out your copyright notice in the Description page of Project Settings.


#include "DayCycleManager.h"

#include "Components/SkyLightComponent.h"

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
	float DayProgress = ElapsedTime / SECONDS_IN_A_DAY;
	float Rotation = 360.f * DayProgress;

	//FRotator NewRotation = FRotator(0.f, Rotation, 0.f);
	//SkyLightComponent->SetWorldRotation(NewRotation);

	FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor(0.25f, 0.1f, 1.0f), FLinearColor(1.0f, 0.9f, 0.6f), FMath::Abs(FMath::Sin(DayProgress * 2 * PI)));
	SkyLightComponent->SetLightColor(Color);
	float Intensity = FMath::Abs(FMath::Sin(FMath::DegreesToRadians(Rotation)));
	SkyLightComponent->SetIntensity(Intensity);
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
