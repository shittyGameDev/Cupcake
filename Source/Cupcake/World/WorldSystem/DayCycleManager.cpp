#include "DayCycleManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/TextBlock.h"
#include "Cupcake/PlayerSystem/CupcakeCharacter.h"
#include "Cupcake/PlayerSystem/NewInventoryComponent.h"
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
	if(Polyephemus)
	{
		Polyephemus->SetActorHiddenInGame(true);
		Polyephemus->SetActorEnableCollision(false);
	}
	PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	PlayerCharacter = Cast<ACupcakeCharacter>(PlayerPawn);	
	DayCycle = 1;
	for (FTimeEvent& Event : TimeEvents)
	{
		BindTimeEvent(Event);
	}
}

// Called every frame
void ADayCycleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedTime += DeltaTime * AccelerateTime;
	/*
	if (ElapsedTime >= SECONDS_IN_A_DAY)
	{
		DayCycle++;
		ElapsedTime = 0;
		UE_LOG(LogTemp, Display, TEXT("New Day %i, Time: %f"), GetCurrentDayNumber(), ElapsedTime);
		DayTransistion();
	}*/
	float DayProgress = ElapsedTime / SECONDS_IN_A_DAY;
	float Rotation = 360.f * DayProgress;
	/*
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
	}*/

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
/*
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
bool ADayCycleManager::CanSleep()
{
	return GetCurrentDayNumber() > LastSleepDay;
}
*/

void ADayCycleManager::RegisterTimeEvent(FTimeEvent& NewEvent)
{
	TimeEvents.Add(NewEvent);
	BindTimeEvent(NewEvent);
}

void ADayCycleManager::DayTransistion(int ZIndex)
{
	if (DayTransitionWidgetClass != nullptr)
	{
		UUserWidget* DayTranistionWidget = CreateWidget<UUserWidget>(GetWorld(), DayTransitionWidgetClass);
		if (DayTranistionWidget != nullptr)
		{
			//PlayerCharacter->DisableMovement();
			DayTranistionWidget->AddToViewport(ZIndex);
			if(UTextBlock* DayText = Cast<UTextBlock>(DayTranistionWidget->GetWidgetFromName(TEXT("DayText"))))
			{
				DayText->SetText(FText::Format(NSLOCTEXT("Game", "DayText", "Day {0}"), FText::AsNumber(GetCurrentDayNumber())));
			}

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DayTranistionWidget]()
			{
				DayTranistionWidget->RemoveFromParent();
				UE_LOG(LogTemp, Warning, TEXT("Ta bort daytransition widgeten"));
				PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				//bDayTransitionTriggered = false;
			}, 3.5f, false);
			if (DayCycle == 2)
			{
				UE_LOG(LogTemp, Warning, TEXT("DayTransition incremented DayCycle to 2, calling RemoveTutorialBarrier"));
				PlayDayTwo();
			}

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

void ADayCycleManager::ShiftDay(int DayTransistionZIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("ShiftDay called. DayCycle: %d"), DayCycle);
	DayCycle++;

	UE_LOG(LogTemp, Warning, TEXT("Daycount: %d"), DayCycle);
    //if (bDayTransitionTriggered)
    /*{
        UE_LOG(LogTemp, Warning, TEXT("DayTransition already triggered for today."));
        return; // Prevent further executions if already done
    }*/

    ApplyInsanity();
    if (PlayerCharacter)
    {
        PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);
    }

	//if (!bDayTransitionTriggered)
	//
		DayTransistion(DayTransistionZIndex);
		PlayerCharacter->SetActorLocation(PlayerSpawnPoint);
		//bDayTransitionTriggered = true; // Set to true to prevent further executions
	//}
/*
    if (DirectionalLight && DirectionalLight->GetLightComponent())
    {
        const float TargetIntensity = 0.0f;
        const float DecreaseAmount = 0.03f; // Adjust decrease amount to ensure it's perceivable each tick

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle, TargetIntensity, DecreaseAmount]()
        {
            // Retrieve current intensity inside the lambda to ensure it's updated each tick
            float CurrentIntensity = DirectionalLight->GetLightComponent()->Intensity;
            float NewIntensity = FMath::Max(CurrentIntensity - DecreaseAmount, TargetIntensity);
            DirectionalLight->GetLightComponent()->SetIntensity(NewIntensity);

            UE_LOG(LogTemp, Warning, TEXT("CurrentIntensity: %f, NewIntensity: %f"), CurrentIntensity, NewIntensity);

            if (NewIntensity <= 0.05f || FMath::IsNearlyZero(NewIntensity, 0.01f))
            {
                GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
                UE_LOG(LogTemp, Warning, TEXT("Light intensity reached near zero, clearing timer."));
                if (!bDayTransitionTriggered)
                {
                    DayTransistion();
                    PlayerCharacter->SetActorLocation(PlayerSpawnPoint);
                    bDayTransitionTriggered = true; // Set to true to prevent further executions
                }
            }
        }, 0.05f, true); // Timer interval set to 0.05 seconds
    }
    else
    {
        DayTransistion();
        PlayerCharacter->EnableMovement();
        bDayTransitionTriggered = true;
    }*/


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

void ADayCycleManager::RemoveTutorialBarrier()
{
	UE_LOG(LogTemp, Warning, TEXT("Förstör stenen"));
	for (AActor* Actor : ActorsBarrier)
	{
		Actor->Destroy();
	}
	PlayStoneSound();
}

void ADayCycleManager::Polyphemus()
{
	if(Polyephemus != nullptr)
	{
		Polyephemus->SetActorHiddenInGame(false);
		Polyephemus->SetActorEnableCollision(true);
	}
}

float ADayCycleManager::GetElapsedTime() const
{
	return ElapsedTime;
}


