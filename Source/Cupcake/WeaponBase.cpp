// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Actors/HealthComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set origo position
	USphereComponent* OrigoPosition = CreateDefaultSubobject<USphereComponent>(TEXT("Origo"));
	OrigoPosition->InitSphereRadius(10.f);
	RootComponent = OrigoPosition;

	// Collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));

	CollisionComponent->SetVisibility(true); // Set visibility to true

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->DoDamage(DamageAmount);
		}
	}
}

void AWeaponBase::EnableWeapon()
{
	SetActorHiddenInGame(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponBase::DisableWeapon()
{
	SetActorHiddenInGame(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}