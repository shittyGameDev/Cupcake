// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Cupcake/Actors/AttributeComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATree::ATree() : IDamageableInterface(Attributes)
{
	PrimaryActorTick.bCanEverTick = false;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(Collider);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(RootComponent);

	ShakeDuration = 0.2f;
	ShakeMagnitude = 2.0f;
	ShakeTimeRemaining = 0.0f;
	bIsShaking = false;

	FallDirection = FVector::ForwardVector; // Default fall direction
}

float ATree::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	SpawnLeafParticle();
	ShakeTree();
	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ATree::ShakeTree()
{
	bIsShaking = true;
	ShakeTimeRemaining = ShakeDuration;
}


void ATree::BeginPlay()
{
	Super::BeginPlay();
	OriginalPosition = GetActorLocation();
}

void ATree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsShaking)
	{
		if (ShakeTimeRemaining > 0)
		{
			const FVector ShakeOffset = UKismetMathLibrary::RandomUnitVector() * ShakeMagnitude;
			SetActorLocation(OriginalPosition + ShakeOffset);
			ShakeTimeRemaining -= DeltaTime;
		}
		else
		{
			bIsShaking = false;
			SetActorLocation(OriginalPosition);
		}
	}
}

