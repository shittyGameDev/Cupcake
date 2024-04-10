// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/Interactable.h"
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
class CUPCAKE_API ADayCycleManager : public AActor, public IInteractable
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
	FVector SpawnTreeLocation;
	UPROPERTY(EditAnywhere)
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere)
	float SleepDurationInHours = 7.f;

	UPROPERTY(EditAnywhere)
	float AccelerateTime = 72.f;

	UPROPERTY(EditAnywhere, Category = "UI")
	UUserWidget* BlackScreenWidget = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlackScreenWidgetClass;
	
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
	void Sleep();
	virtual void Interact_Implementation();

	bool CanSleep();

	void RegisterTimeEvent(const FTimeEvent& NewEvent);
	UFUNCTION()
	void SpawnTreeEvent();

private:
	char DayCycle = 0;
	float ElapsedTime = 0.f;
	float const SECONDS_IN_A_DAY = 86400.f;
	int LastSleepDay = -1;
	bool bHasSlept = false;
	APlayerController* PlayerController;
	APawn* PlayerPawn;
	
	TArray<FTimeEvent> TimeEvents;
};