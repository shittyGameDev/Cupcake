// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "GameFramework/Actor.h"
#include "Engine/SkyLight.h"
#include "DayCycleManager.generated.h"

class ACupcakeCharacter;
class AStaticMeshActor;


DECLARE_DYNAMIC_DELEGATE(FTimeSpecificEvent);

USTRUCT(BlueprintType)
struct FTimeEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Event")
	int Day = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Event")
	int Hour = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Event")
	int Minute = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Event")
	FString FunctionName;

	FTimeSpecificEvent EventDelegate;
};


UCLASS()
class CUPCAKE_API ADayCycleManager : public AActor
{
	
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADayCycleManager();

	UFUNCTION(BlueprintCallable)
	FVector GetPlayerSpawnLocation() { return PlayerSpawnPoint; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMesh* CubeMesh;
	UPROPERTY(EditAnywhere)
	FVector SpawnTreeLocation;
	UPROPERTY(EditAnywhere)
	FRotator SpawnRotation;
	UPROPERTY(EditAnywhere, Category = "SpawnPoint")
	FVector PlayerSpawnPoint;
	UPROPERTY(EditAnywhere)
	float SleepDurationInHours = 7.f;

	UPROPERTY(EditAnywhere)
	float AccelerateTime = 72.f;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	UUserWidget* BlackScreenWidget = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlackScreenWidgetClass;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DayTransitionWidgetClass;
	UPROPERTY(EditAnywhere, Category= "Insanity")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Management")
	TArray<FTimeEvent> TimeEvents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Barrier")
	TArray<AActor*> ActorsBarrier;
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
	//void ShiftTime(float Time);
	//void Sleep();
	//virtual void Interact_Implementation();

	//bool CanSleep();
	void DayTransistion(int ZIndex = 1000);
	void BindTimeEvent(FTimeEvent& Event);

	void ShiftDayBind()
	{
		ShiftDay();
	}
	UFUNCTION(BlueprintCallable)
	void ShiftDay(int DayTransistionZIndex = 1000);
	
	void RegisterTimeEvent(FTimeEvent& NewEvent);
	void RestoreLightIntensity();
	UFUNCTION()
	void SpawnTreeEvent();
	UFUNCTION()
	void ApplyInsanity();
	UFUNCTION()
	void RemoveTutorialBarrier();
	
	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const;

private:
	char DayCycle = 0;
	float ElapsedTime = 0.f;
	float const SECONDS_IN_A_DAY = 86400.f;
	int LastSleepDay = -1;
	float OrginalIntensity = 3.f;
	bool bHasSlept = false;
	bool bSleepWidgetActive = false;
	bool bDayTransistionScheduled = false;
	bool bIsReactivationScheduled = false;
	bool bDayTransitionTriggered = false;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	APawn* PlayerPawn;
	UPROPERTY()
	ACupcakeCharacter* PlayerCharacter;
	
	
};