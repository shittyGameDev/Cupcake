// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Cupcake/Actors/AttributeComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"

AEnemyCharacter::AEnemyCharacter() : IDamageableInterface(Attributes)
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
	if (PerceptionSystem)
	{
		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator, DamageAmount, GetActorLocation(), GetActorLocation());
	}
	
	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacter::OnDeath_Implementation()
{
	Weapon->Destroy();
	IDamageableInterface::OnDeath_Implementation();
	Destroy();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponBlueprint)
	{
		// Spawn the weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());
		Weapon->SetOwner(this);
		Weapon->HideWeapon();
		Weapon->Unequip();

		// Attach the weapon
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy %s: Missing BP_Weapon!"), *GetActorLabel());
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemyCharacter::Attack()
{
	if (!Weapon) return;
	
	//Weapon->Equip();

	UE_LOG(LogTemp, Warning, TEXT("Attack"));
}

void AEnemyCharacter::OnAttackFinished() const
{
	if (Weapon)
	{
		Weapon->Unequip();
	}
}