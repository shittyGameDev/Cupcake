// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Actors/DamageableInterface.h"
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
	
	AWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;

	FTimerHandle TimerHandle_AttackFinished;


	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditAnywhere, Category= "Drop | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Drop | Item Reference")
	UBaseItem* ItemReference;

public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void DoAttack();
	UFUNCTION(BlueprintCallable)
	void DoSweepAttack();
	void OnAttackFinished();

};
