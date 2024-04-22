// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRevealTrigger.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMapRevealTrigger::AMapRevealTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->InitBoxExtent(FVector(50.f, 50.f, 50.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMapRevealTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AMapRevealTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapRevealTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapRevealTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is valid, not this trigger, and the other component is also valid
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// Check if the overlapping actor is the player
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			// Retrieve the user widget subsystem from the game instance
			if (UUserWidgetSubsystem* WidgetSubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UUserWidgetSubsystem>())
			{
				// Retrieve the specific map widget by its name
				if (UUserWidget* MapWidget = Cast<UUserWidget>(WidgetSubsystem->GetWidget(FName("UI_MapWidget"))))
				{
					// Call the function to reveal the map part, assuming MapPartTag is a FName member variable of this class
					FString Command = FString::Printf(TEXT("RevealMapPart %s"), *MapPartTag.ToString());
					MapWidget->CallFunctionByNameWithArguments(*Command, nullptr, nullptr, true);
				}
			}
		}
	}
}
	



