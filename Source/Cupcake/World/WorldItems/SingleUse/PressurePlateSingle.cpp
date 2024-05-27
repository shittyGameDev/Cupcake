// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlateSingle.h"

#include "Components/BoxComponent.h"


APressurePlateSingle::APressurePlateSingle()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsMoving = false;
	ElapsedTime = 0.0f;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MushroomTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MushroomTriggerMesh"));
	MushroomTriggerMesh->SetupAttachment(RootComponent);
	MushroomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MushroomTrigger"));
	MushroomTrigger->SetupAttachment(MushroomTriggerMesh);
}

void APressurePlateSingle::BeginPlay()
{
	Super::BeginPlay();

	MushroomTrigger->OnComponentBeginOverlap.AddDynamic(this, &APressurePlateSingle::OnOverlapMushroomBegin);
}

void APressurePlateSingle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		ElapsedTime += DeltaTime;
		float Alpha = ElapsedTime / MoveDuration;

		for (AActor* Part : DoorParts)
		{
			if (Part)
			{
				Part->SetActorLocation(Part->GetActorLocation() + MovementOffset);
			}
		}

		if (ElapsedTime >= MoveDuration)
		{
			bIsMoving = false;
		}
	}
}

void APressurePlateSingle::OnOverlapMushroomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AActor* Part : DoorParts)
	{
		if (Part)
		{
			UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Part->GetRootComponent());
			if (RootComp && RootComp->Mobility == EComponentMobility::Static)
			{
				RootComp->SetMobility(EComponentMobility::Movable);
				UE_LOG(LogTemp, Warning, TEXT("Set mobility to movable for %s"), *Part->GetName());
			}
		}
	}

	bIsMoving = true;
}