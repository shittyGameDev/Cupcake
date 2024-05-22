// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "TreeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKE_API ATreeEnemy : public AEnemyCharacter
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	// Hand weapon
	UPROPERTY(Blueprintable, BlueprintGetter=GetHandWeapon)
	AWeaponBase* HandWeapon;
	UFUNCTION(BlueprintGetter)
	AWeaponBase* GetHandWeapon() const { return HandWeapon; }
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> HandWeaponBlueprint;

	// AoE Weapom
	UPROPERTY(Blueprintable, BlueprintGetter=GetAoEWeapon)
	AWeaponBase* AoEWeapon;
	UFUNCTION(BlueprintGetter)
	AWeaponBase* GetAoEWeapon() const { return AoEWeapon; }
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> AoEWeaponBlueprint;

	FTimerHandle TimerHandle_AoE;
	
public:
	UFUNCTION(BlueprintCallable)
	void SwipeAttack();

	UFUNCTION(BlueprintCallable)
	void AreaDamage();
	void OnAreaDamageFinished();
};
