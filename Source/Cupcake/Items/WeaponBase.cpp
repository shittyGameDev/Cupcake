// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "../Actors/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Weapon Mesh component
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// Weapon Collision component
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionProfileName(TEXT("Trigger"));

	// Bind overlap event
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponOverlap);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->GetComponentByClass<UAttributeComponent>())
		{
			if (GetOwner() != nullptr && GetOwner() != OtherActor)
			{
				UGameplayStatics::ApplyDamage(
					OtherActor,
					DamageAmount,
					GetOwner()->GetInstigatorController(),
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

void AWeaponBase::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeaponBase::Equip()
{
	// Should show weapon and enable collision
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponBase::Unequip()
{
	// Can still be shown but shouldn't deal damage, therefore disable collison
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBase::ShowWeapon()
{
	WeaponBox->SetActive(true);
	SetActorHiddenInGame(false);
}


void AWeaponBase::HideWeapon()
{
	WeaponBox->SetActive(false);
	SetActorHiddenInGame(true);
}