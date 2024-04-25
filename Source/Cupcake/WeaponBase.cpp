// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Actors\DamagableComponent.h"
#include "Kismet/GameplayStatics.h"


AWeaponBase::AWeaponBase()
{
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

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnSphereOverlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AWeaponBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		UDamagableComponent* DamagableComponent = Cast<UDamagableComponent>(OtherActor);
		if (DamagableComponent)
		{
			UGameplayStatics::ApplyDamage(
				OtherActor,
				DamageAmount,
				GetOwner()->GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
			);
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