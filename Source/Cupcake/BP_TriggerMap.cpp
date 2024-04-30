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
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (OtherActor == PlayerCharacter)
	{
		UTheMapHandler* MyMidget = UTheMapHandler::GetInstance();
		UE_LOG(LogTemp, Warning, TEXT("DU KROCKAR I MIG"));
		// Hitta din UserWidget och gör en cast till rätt typ, antag att den heter UMyUserWidget
		//UTheMapHandler* MyWidget = Cast<UTheMapHandler>(LinkedWidget);
		if (MyMidget)
		{
			switch (id)
			{
			case 1:
				UE_LOG(LogTemp, Warning, TEXT("HEJ"));
				
				if(MyMidget->Image1)
				{
					MyMidget->Image1->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp, Warning, TEXT("TEXT1"));
				}
				break;
			case 2:
				if(MyMidget->Image2)
				{
					MyMidget->Image2->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp, Warning, TEXT("TEXT1"));
				}
			case 3:
				
				if(MyMidget->Image3 == nullptr)
				{
					UE_LOG(LogTemp, Warning, TEXT("HEJ"));
				}
				if(MyMidget->Image3)
				{
					MyMidget->Image3->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp, Warning, TEXT("TEXT3"));
				}

				break;
			case 4:
				if(MyMidget->Image4)
				{
					MyMidget->Image4->SetVisibility(ESlateVisibility::Hidden);
					UE_LOG(LogTemp, Warning, TEXT("TEXT1"));
				}
			default:
				break; // If no valid id, do nothing (Destroy removed)
			}
		}
	}
}


