// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeEnemy.h"

void ATreeEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HandWeaponBlueprint)
	{
		// Spawn
		HandWeapon = GetWorld()->SpawnActor<AWeaponBase>(HandWeaponBlueprint, GetActorLocation(), GetActorRotation());
		HandWeapon->SetOwner(this);
		HandWeapon->Unequip();

		// Attach the weapon
		HandWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HandWeaponSocket"));

		// Hide weapons
		Weapon->HideWeapon();
		HandWeapon->HideWeapon();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy %s: Missing BP_Weapon!"), *GetActorNameOrLabel());
	}
}

void ATreeEnemy::SwipeAttack()
{
	if (!HandWeapon) return;

	UE_LOG(LogTemp, Display, TEXT("Swipe attack"));
}

void ATreeEnemy::AreaDamage()
{
	AoEWeapon->Equip();

	GetWorldTimerManager().SetTimer(TimerHandle_AoE, this, &ATreeEnemy::OnAreaDamageFinished, 1.0f, false, 2.0f);
}

void ATreeEnemy::OnAreaDamageFinished()
{
	AoEWeapon->Unequip();
}
