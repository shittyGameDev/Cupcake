// Fill out your copyright notice in the Description page of Project Settings.


#include "DayCycleManager.h"

#include "FunctionalUIScreenshotTest.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SkyLightComponent.h"
#include "Engine/StaticMeshActor.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

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

	FTimeEvent TreeSpawnEvent;
	TreeSpawnEvent.Day = 0;
	TreeSpawnEvent.Hour = 0;
	TreeSpawnEvent.Minute = 1;
	TreeSpawnEvent.EventDelegate.BindUFunction(this, FName("SpawnTreeEvent"));
	RegisterTimeEvent(TreeSpawnEvent);
	
}

// Called every frame
void ADayCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime * AccelerateTime;
	
	/*if (!bHasSlept && ElapsedTime > 5 * AccelerateTime)
	{
		Sleep();
		bHasSlept = true;
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("Time: %f"), ElapsedTime);
	if (ElapsedTime >= SECONDS_IN_A_DAY)
	{
		DayCycle++;
		ElapsedTime = 0;
		UE_LOG(LogTemp, Display, TEXT("New Day %i, Time: %f"), GetCurrentDayNumber(), ElapsedTime);
	}
	float DayProgress = ElapsedTime / SECONDS_IN_A_DAY;
	float Rotation = 360.f * DayProgress;

	if (SkyLight && SkyLight->GetLightComponent() && DirectionalLight && DirectionalLight->GetLightComponent())
	{
		// Uppdatera rotation
		FRotator NewRotation = FRotator(-30.f, Rotation, 0.f);
		DirectionalLight->SetActorRotation(NewRotation);

		// Uppdatera färg
		FLinearColor Color = FLinearColor::LerpUsingHSV(FLinearColor(0.25f, 0.1f, 1.0f), FLinearColor(1.0f, 0.9f, 0.6f), FMath::Abs(FMath::Sin(DayProgress * 2 * PI)));
		SkyLight->GetLightComponent()->SetLightColor(Color);

		// Uppdatera intensitet
		float Intensity = FMath::Abs(FMath::Sin(FMath::DegreesToRadians(Rotation)));
		SkyLight->GetLightComponent()->SetIntensity(Intensity);
	}

	//check för om ett event ska ske nu
	int CurrentDay = GetCurrentDayNumber();
	//UE_LOG(LogTemp, Warning, TEXT("Day: %f"), CurrentDay);
	int CurrentHour = GetHour();
	//UE_LOG(LogTemp, Warning, TEXT("Hour: %f"), CurrentHour);
	int CurrentMinute = GetMinutes();
	//UE_LOG(LogTemp, Warning, TEXT("Minute: %f"), CurrentMinute);
	for(int i = TimeEvents.Num() - 1; i >= 0; i--)
	{
		if(TimeEvents[i].Day == CurrentDay && TimeEvents[i].Hour == CurrentHour && TimeEvents[i].Minute == CurrentMinute)
		{
			//trigga det unika eventet
			TimeEvents[i].EventDelegate.ExecuteIfBound();

			TimeEvents.RemoveAt(i);
		}
	} 
}

int ADayCycleManager::GetCurrentDayNumber()
{
	return DayCycle;
}

int ADayCycleManager::GetHour()
{
	return static_cast<int>((ElapsedTime / 3600)) % 24;
}

int ADayCycleManager::GetMinutes()
{
	return static_cast<int>((ElapsedTime / 60)) % 60;
}

void ADayCycleManager::ShiftTime(float Time)
{
	float OldElapsedTime = ElapsedTime; // Spara det gamla värdet för att jämföra
	ElapsedTime += Time;
    
	int daysPassed = FMath::FloorToInt(ElapsedTime / SECONDS_IN_A_DAY);
	DayCycle += daysPassed;
    
	ElapsedTime -= daysPassed * SECONDS_IN_A_DAY;

	UE_LOG(LogTemp, Warning, TEXT("ShiftTime called. Old Time: %f, New Time: %f, Days Passed: %d"), OldElapsedTime, ElapsedTime, DayCycle);
}

void ADayCycleManager::Sleep()
{
	UE_LOG(LogTemp, Display, TEXT("Hour before: %i"), GetHour());
	ShiftTime(SleepDurationInHours * 60 * 60);
	UE_LOG(LogTemp, Display, TEXT("Hour after: %i"), GetHour());
	
	if (BlackScreenWidget)
	{
		BlackScreenWidget->RemoveFromParent();
		BlackScreenWidget = nullptr; 
	}
	
	if (BlackScreenWidgetClass != nullptr)
	{
		
		BlackScreenWidget = CreateWidget<UUserWidget>(GetWorld(), BlackScreenWidgetClass);
		if (BlackScreenWidget != nullptr)
		{
			BlackScreenWidget->AddToViewport(1000);
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (BlackScreenWidget)
				{
					BlackScreenWidget->RemoveFromParent();
					BlackScreenWidget = nullptr; 
				}
			}, 3.0f, false);
		}
	}
}



void ADayCycleManager::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Fuck off you think you can sleep idiot?"));
	if (CanSleep())
	{
		UE_LOG(LogTemp, Warning, TEXT("I guess you can sleep now :)"));
		Sleep();
		LastSleepDay = GetCurrentDayNumber();
	}
}

void ADayCycleManager::RegisterTimeEvent(const FTimeEvent& NewEvent)
{
	TimeEvents.Add(NewEvent);
}

bool ADayCycleManager::CanSleep()
{
	return GetCurrentDayNumber() > LastSleepDay;
}

void ADayCycleManager::SpawnTreeEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawned Cube!"));
	if(CubeMesh)
	{
		AStaticMeshActor* CubeActor = GetWorld()->SpawnActor<AStaticMeshActor>(SpawnTreeLocation, SpawnRotation);
		if(CubeActor)
		{
			UE_LOG(LogTemp, Display, TEXT("Ran inner function"));
			CubeActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			CubeActor->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
			
			CubeActor->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
	}

}
