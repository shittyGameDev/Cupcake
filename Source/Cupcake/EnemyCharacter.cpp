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
		Weapon->SetOwner(this);
		Weapon->ShowWeapon();

		// Attach the weapon to the mannequin's socket with the same rotation
		if (GetMesh()->DoesSocketExist(TEXT("WeaponSocket")))
		{
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		}
		else
		{
			Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}

		Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s: Missing Weapon!"), *GetActorLabel());
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemyCharacter::DoAttack()
{
	if (!Weapon) return;
	
	Weapon->Equip();

	UE_LOG(LogTemp, Warning, TEXT("Swing"));

	// The attack logic
	
	// Duration for attack
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &AEnemyCharacter::OnAttackFinished, 0.2f, false);
}

void AEnemyCharacter::DoSweepAttack()
{
	if (!Weapon) return;

	Weapon->Equip();

	UE_LOG(LogTemp, Warning, TEXT("Sweep"));
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &AEnemyCharacter::OnAttackFinished, 0.3f, false);
}

void AEnemyCharacter::OnAttackFinished() const
{
	if (Weapon)
	{
		// Disable weapon collider
		Weapon->Unequip();
	}
}