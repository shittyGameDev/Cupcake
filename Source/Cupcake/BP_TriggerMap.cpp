// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_TriggerMap.h"

#include "TheMapHandler.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"


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
	
}

// Called every frame
void ABP_TriggerMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABP_TriggerMap::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("DU KROCKAR I MIG"));
		// Hitta din UserWidget och gör en cast till rätt typ, antag att den heter UMyUserWidget
		UTheMapHandler* MyWidget = Cast<UTheMapHandler>(LinkedWidget);
		if (MyWidget)
		{
			// Använd taggen för att hitta den rätta svarta bilden i widgeten
			FString TagName = Tags[0].ToString(); // Antag att vi har en tag som definierar vilken bild som ska påverkas
			UE_LOG(LogTemp, Warning, TEXT("Tagname: %s"), *TagName);
			MyWidget->SetImageOpacity(FName(*TagName), 0.0f);	
			//Destroy();
			
		}
	}
}


