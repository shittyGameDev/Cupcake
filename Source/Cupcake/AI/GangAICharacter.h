// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GangAIManager.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "Cupcake/WeaponBase.h"
#include "GangAICharacter.generated.h"

class UAIPerceptionComponent;
class UNiagaraComponent;
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

	UFUNCTION(BlueprintCallable)
	void EnableChasing();

	UFUNCTION(BlueprintCallable)
	void InitiateAttack(AActor* Actor);
	
	bool IsChasing() const { return bIsChasing; }

	bool IsAttacking() const { return bIsAttacking; }

	FVector GetRandomPatrolPoint();

	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;
	UPROPERTY(EditAnywhere)
	float PatrolRadius;
	UPROPERTY(EditAnywhere)
	float ChaseDistance;
	UPROPERTY(EditAnywhere, Category= "AI")
	float AttackDistance;
	UPROPERTY(EditAnywhere, Category= "AI")
	float DashDistance;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector CurrentPatrolPoint;
	UPROPERTY(EditAnywhere);
	AWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;
	
	FTimerHandle TimerHandle_PreAttack; 
	FTimerHandle TimerHandle_AttackFinished;
	FTimerHandle TimerHandle_Cooldown;

	FVector TargetAttackPosition;
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* NiagaraComponent; 

protected:
	//AAIController* AIController;
	bool bIsChasing;
	bool bIsAttacking;
	AActor* Player;
	//PROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	//UAIPerceptionComponent* PerceptionComponent;

	AGangAIManager* AIManager;
	
};
