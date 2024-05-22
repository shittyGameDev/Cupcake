// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/WeaponBase.h"
#include "../Actors/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class CUPCAKE_API AEnemyCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath_Implementation() override;
protected:
	virtual void BeginPlay() override;

	// Head weapon
	UPROPERTY(Blueprintable, BlueprintGetter=GetWeapon)
	AWeaponBase* Weapon;
	UFUNCTION(BlueprintGetter)
	AWeaponBase* GetWeapon() const { return Weapon; }
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;
	
public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Attack();
	void OnAttackFinished() const;
};
