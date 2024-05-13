// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_TriggerMap.h"
#include "TheMapObject.h" 
#include "TheMapHandler.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../PlayerSystem/CupcakeCharacter.h"


// Sets default values
ABP_TriggerMap::ABP_TriggerMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABP_TriggerMap::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABP_TriggerMap::BeginPlay()
{
	Super::BeginPlay();
	
	if(TheMapHandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test"));
	}
	
}

// Called every frame
void ABP_TriggerMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABP_TriggerMap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Hämta referensen till ATheMapObject
	ATheMapObject* MapObject = Cast<ATheMapObject>(UGameplayStatics::GetActorOfClass(GetWorld(), ATheMapObject::StaticClass()));
	if (MapObject && OtherActor && OtherActor->IsA(ACupcakeCharacter::StaticClass()))
	{
		if(!ActivePickupNotificationWidget && PickupNotificationWidgetClass)
		{
			ActivePickupNotificationWidget = CreateWidget<UUserWidget>(GetWorld(), PickupNotificationWidgetClass);
			if (ActivePickupNotificationWidget)
			{
				ActivePickupNotificationWidget->AddToViewport(100);
				UTheMapHandler* MapHandler = Cast<UTheMapHandler>(MapObject->MapWidget); // Casta till din specifika widget klass
				if (MapHandler)
				{
					// Använd någon logik för att bestämma vilken bild som ska döljas baserat på 'id' eller annan logik
					MapHandler->HideImage(id); // Antag att HideImage är en metod i UTheMapHandler som tar ett ID och döljer motsvarande bild.
					UE_LOG(LogTemp, Warning, TEXT("Triggered image hide"));
					Destroy();
				}
			}
		}

	}
}
	



