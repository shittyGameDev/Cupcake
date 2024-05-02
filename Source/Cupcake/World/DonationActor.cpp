// Fill out your copyright notice in the Description page of Project Settings.


#include "DonationActor.h"

#include "ObeliskActor.h"

// Sets default values
ADonationActor::ADonationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void ADonationActor::BeginPlay()
{
	Super::BeginPlay();

	if(ObeliskActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("DESTROYING ACTOR"));
		ObeliskActor->OnDonationGoalReached.AddUObject(this, &ADonationActor::DestroyActor);
	}

	UE_LOG(LogTemp, Warning, TEXT("Obelisk Actor: %p"), ObeliskActor);
}

// Called every frame
void ADonationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADonationActor::DestroyActor()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTROYING ACTOR"));
	Destroy();
}

