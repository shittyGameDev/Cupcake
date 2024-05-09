// Fill out your copyright notice in the Description page of Project Settings.


#include "CaveLightningActor.h"

#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "Engine/DirectionalLight.h"

ACaveLightningActor::ACaveLightningActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACaveLightningActor::ToggleLightIntensity);

	bIsCaveLightActive = false; // Initialize the light state
}

void ACaveLightningActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACaveLightningActor::ToggleLightIntensity(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bIsCaveLightActive)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(NormalLight);
		bIsCaveLightActive = false;
	}
	else
	{
		DirectionalLight->GetLightComponent()->SetIntensity(CaveLight);
		bIsCaveLightActive = true;
	}
}




