// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Actors/AttributeComponent.h"
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

		// Optionally, attach the weapon to the character
		//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));

		Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		Weapon->DisableWeapon();
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemyCharacter::DoAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	if (!Weapon) return;
    
	// Attach the weapon to the character, assuming you have a socket named "WeaponSocket" on the character
	if (!Weapon->GetRootComponent()->IsAttachedTo(GetMesh()))
	{
		//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ik_hand_root"));
		UE_LOG(LogTemp, Warning, TEXT("Attached"));
	}
	Weapon->SetOwner(this);
	Weapon->EnableWeapon(); // Enable the weapon

	// Set a timer to disable the weapon after a short duration, simulating an attack duration
	// Assuming an attack takes 1 second; adjust this duration as needed
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &AEnemyCharacter::OnAttackFinished, 0.2f, false);
}

void AEnemyCharacter::OnAttackFinished()
{
	if (Weapon)
	{
		Weapon->DisableWeapon(); // Disable the weapon after the attack is complete
	}
}