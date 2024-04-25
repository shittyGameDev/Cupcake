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

}
	



