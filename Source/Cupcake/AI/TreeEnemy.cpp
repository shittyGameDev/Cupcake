// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeEnemy.h"

void ATreeEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HandWeaponBlueprint && AoEWeaponBlueprint)
	{
		// Spawn
		HandWeapon = InitiateWeapon(HandWeapon, HandWeaponBlueprint, TEXT("HandWeaponSocket"), this);
		AoEWeapon = InitiateWeapon(AoEWeapon, AoEWeaponBlueprint, TEXT("WeaponSocket"), this);
		
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
	if (!AoEWeapon) return;

	UE_LOG(LogTemp, Warning, TEXT("Ran"));
	
	AoEWeapon->ShowWeapon();
	AoEWeapon->Equip();

	GetWorldTimerManager().SetTimer(TimerHandle_AoE, this, &ATreeEnemy::OnAreaDamageFinished, 1.0f, false, 2.0f);
}

void ATreeEnemy::OnAreaDamageFinished()
{
	AoEWeapon->Unequip();
}
