// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Actors/HealthComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMeshComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetupAttachment(RootComponent);

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