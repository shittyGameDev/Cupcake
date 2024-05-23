// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

#include "Components/BoxComponent.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MushroomTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MushroomTriggerMesh"));
	MushroomTriggerMesh->SetupAttachment(RootComponent);
	MushroomTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MushroomTrigger"));
	MushroomTrigger->SetupAttachment(MushroomTriggerMesh);

	PlayerTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerTriggerMesh"));
	PlayerTriggerMesh->SetupAttachment(RootComponent);
	PlayerTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerTrigger"));
	PlayerTrigger->SetupAttachment(PlayerTriggerMesh);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorMesh->SetupAttachment(RootComponent);
	

}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	MushroomTrigger->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapMushroomBegin);
	MushroomTrigger->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapMushroomEnd);

	PlayerTrigger->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapPlayerBegin);
	PlayerTrigger->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapPlayerEnd);
}

void APressurePlate::OnOverlapMushroomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Triggered the mushroom"));
	bIsMushroomTriggered = true;
}

void APressurePlate::OnOverlapMushroomEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsMushroomTriggered = false;
}

void APressurePlate::OnOverlapPlayerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsPlayerTriggered = true;
	UE_LOG(LogTemp, Warning, TEXT("U Triggered the player"));
	UnlockDoor();
}

void APressurePlate::OnOverlapPlayerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsPlayerTriggered = false;
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

