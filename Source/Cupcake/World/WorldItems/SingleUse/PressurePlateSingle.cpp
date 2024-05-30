// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlateSingle.h"

#include "NiagaraComponent.h"
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

	Particles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particles"));
	Particles->SetupAttachment(RootComponent);
}

void APressurePlateSingle::BeginPlay()
{
	Super::BeginPlay();

	MushroomTrigger->OnComponentBeginOverlap.AddDynamic(this, &APressurePlateSingle::OnOverlapMushroomBegin);

	if (Door)
	{
		InitialDoorLocation = Door->GetActorLocation();
	}
	Particles->SetActive(false);
}

void APressurePlateSingle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		ElapsedTime += DeltaTime;
		float Alpha = ElapsedTime / MoveDuration;

		if (Door)
		{
			FVector StartLocation = InitialDoorLocation;
			FVector EndLocation = InitialDoorLocation + MovementOffset;

			FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
			Door->SetActorLocation(NewLocation);
		}

		if (ElapsedTime >= MoveDuration)
		{
			bIsMoving = false;
			Door->SetActorLocation(InitialDoorLocation + MovementOffset);
			Particles->SetActive(false);
		}
	}
}

void APressurePlateSingle::OnOverlapMushroomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Door)
	{
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Door->GetRootComponent());
		if (RootComp && RootComp->Mobility == EComponentMobility::Static)
		{
			RootComp->SetMobility(EComponentMobility::Movable);
			UE_LOG(LogTemp, Warning, TEXT("Set mobility to movable for %s"), *Door->GetName());
		}
	}
	PlayAudio();
	bIsMoving = true;
	Particles->SetActive(true);
}