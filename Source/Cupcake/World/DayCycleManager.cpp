#include "DayCycleManager.h"

#include "Components/SkyLightComponent.h"
#include "Components/TextBlock.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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

	PlayerController = GetWorld()->GetFirstPlayerController();
	
	for (FTimeEvent& Event : TimeEvents)
	{
		BindTimeEvent(Event);
	}

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacter = Cast<ACupcakeCharacter>(PlayerPawn);
}

// Called every frame
void ADayCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime * AccelerateTime;
	//UE_LOG(LogTemp, Warning, TEXT("Time in hours: %i"), GetHour());
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
		DayTransistion();
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
	int DayBeforeSleep = GetCurrentDayNumber();
	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement disabled"));
		PlayerCharacter->DisableMovement();
	}
	
	UE_LOG(LogTemp, Display, TEXT("Hour before: %i"), GetHour());
	ShiftTime(SleepDurationInHours * 60 * 60);
	int DayAfterSleep = GetCurrentDayNumber(); //fortsätta här imornS
	if (DayAfterSleep == DayBeforeSleep)
	{
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
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DayBeforeSleep]()
				{
					if (BlackScreenWidget)
					{
						BlackScreenWidget->RemoveFromParent();
						BlackScreenWidget = nullptr;
					
						if (PlayerCharacter)
						{
							UE_LOG(LogTemp, Warning, TEXT("Enable Movement"));
							PlayerCharacter->EnableMovement();
						}

					}
				}, 3.0f, false);
			}
		}
	}else
	{
		DayTransistion();
	}
	UE_LOG(LogTemp, Display, TEXT("Hour after: %i"), GetHour());
	

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

void ADayCycleManager::RegisterTimeEvent(FTimeEvent& NewEvent)
{
	TimeEvents.Add(NewEvent);
	BindTimeEvent(NewEvent);
}

bool ADayCycleManager::CanSleep()
{
	return GetCurrentDayNumber() > LastSleepDay;
}

void ADayCycleManager::DayTransistion()
{
	if (DayTransitionWidgetClass != nullptr)
	{
		UUserWidget* DayTranistionWidget = CreateWidget<UUserWidget>(GetWorld(), DayTransitionWidgetClass);
		if (DayTranistionWidget != nullptr)
		{
			PlayerCharacter->DisableMovement();
			DayTranistionWidget->AddToViewport(1000);
			if(UTextBlock* DayText = Cast<UTextBlock>(DayTranistionWidget->GetWidgetFromName(TEXT("DayText"))))
			{
				DayText->SetText(FText::Format(NSLOCTEXT("Game", "DayText", "Day {0}"), FText::AsNumber(GetCurrentDayNumber())));
			}

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DayTranistionWidget]()
			{
				DayTranistionWidget->RemoveFromParent();
				PlayerCharacter->EnableMovement();
			}, 3.0f, false);
		}
	}
}

void ADayCycleManager::BindTimeEvent(FTimeEvent& Event)
{
	if (!Event.FunctionName.IsEmpty())
	{
		Event.EventDelegate.BindUFunction(this, FName(*Event.FunctionName));
	}
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

void ADayCycleManager::ApplyInsanity()
{
	if (CameraShakeClass && PlayerController && SkyLight && SkyLight->GetLightComponent())
	{
		FLinearColor Magenta(255.f, 0.f, 231.f);
		SkyLight->GetLightComponent()->SetLightColor(Magenta);
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 50.f;
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeClass);

		bool bIsLightPink = true;

		FTimerHandle FlickerLightTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(FlickerLightTimerHandle, [this, &bIsLightPink, Magenta]()
		{
			FLinearColor NewColor = bIsLightPink ? FLinearColor::White : Magenta;
			SkyLight->GetLightComponent()->SetLightColor(NewColor);
			bIsLightPink = !bIsLightPink;
		}, 0.01f, true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, FlickerLightTimerHandle]() mutable
		{
			PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.f;
			GetWorld()->GetTimerManager().ClearTimer(FlickerLightTimerHandle);
			SkyLight->GetLightComponent()->SetLightColor(FLinearColor::White);
		}, 3.0f, false);
	}
}
