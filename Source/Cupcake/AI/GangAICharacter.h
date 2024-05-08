// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GangAIController.h"
#include "GangAIManager.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "Cupcake/WeaponBase.h"
#include "GangAICharacter.generated.h"

class UAIPerceptionComponent;

UCLASS()
class AGangAICharacter : public ACharacter,  public IDamageableInterface
{
	GENERATED_BODY()

public:
	AGangAICharacter();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser);

	UFUNCTION()
	virtual void OnDeath_Implementation();

	UFUNCTION()
	void OnDamage_Implementation();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartChasing(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void Patrol();

	UFUNCTION(BlueprintCallable)
	void ReturnToPatrol();

	UFUNCTION(BlueprintCallable)
	void DoAttack();

	UFUNCTION(BlueprintCallable)
	void OnAttackFinished();

	bool IsChasing() const { return bIsChasing; }

	FVector GetRandomPatrolPoint();

	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere)
	float PatrolRadius;
	UPROPERTY(EditAnywhere)
	float ChaseDistance;
	UPROPERTY(EditAnywhere, Category= "AI")
	float AttackDistance;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector CurrentPatrolPoint;
	UPROPERTY(EditAnywhere);
	AWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;
	
	FTimerHandle TimerHandle_PreAttack; 
	FTimerHandle TimerHandle_AttackFinished; 

protected:
	//AAIController* AIController;
	bool bIsChasing;
	bool bIsAttacking;
	//PROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	//UAIPerceptionComponent* PerceptionComponent;

	AGangAIManager* AIManager;
	
};
