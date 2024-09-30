// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GangAIManager.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "GangAICharacter.generated.h"

class AWeaponBase;
class UAIPerceptionComponent;
class UNiagaraComponent;

// Define the EAIState enumeration
UENUM(BlueprintType)
enum class EAIState : uint8
{
    Patrolling UMETA(DisplayName = "Patrolling"),
    Chasing UMETA(DisplayName = "Chasing"),
    Attacking UMETA(DisplayName = "Attacking"),
    Returning UMETA(DisplayName = "Returning"),
    Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class AGangAICharacter : public ACharacter, public IDamageableInterface
{
    GENERATED_BODY()

public:
    AGangAICharacter();

    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser) override;

    UFUNCTION(BlueprintImplementableEvent)
    void StartAttack();

    UFUNCTION()
    virtual void OnDeath_Implementation() override;

    UFUNCTION()
    virtual void OnDamage_Implementation() override;

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

    UFUNCTION(BlueprintImplementableEvent)
    void PlayHurtSound();

    bool IsChasing() const { return bIsChasing; }
    bool IsAttacking() const { return bIsAttacking; }

    // Getter for CurrentState
    EAIState GetCurrentState() const { return CurrentState; }

    FVector GetRandomPatrolPoint();

    // AI properties
    UPROPERTY(EditAnywhere)
    FVector SpawnLocation;

    UPROPERTY(EditAnywhere)
    float PatrolRadius;

    UPROPERTY(EditAnywhere)
    float ChaseDistance;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDistance;

    UPROPERTY(EditAnywhere, Category = "AI")
    float DashDistance;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    FVector CurrentPatrolPoint;

    UPROPERTY(Blueprintable, BlueprintGetter = GetWeapon)
    AWeaponBase* Weapon;

    UFUNCTION(BlueprintGetter)
    AWeaponBase* GetWeapon() const { return Weapon; }

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AWeaponBase> WeaponBlueprint;

    UPROPERTY(EditDefaultsOnly, Category = "DamageEffects")
    UMaterialInterface* HitMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "DamageEffects")
    UMaterialInterface* NormalMaterial;

    FTimerHandle TimerHandle_PreAttack;
    FTimerHandle TimerHandle_AttackFinished;
    FTimerHandle TimerHandle_Cooldown;

    FVector TargetAttackPosition;

    UPROPERTY(EditAnywhere)
    UNiagaraComponent* NiagaraComponent;

protected:
    // AI State
    EAIState CurrentState;

    bool bIsChasing;
    bool bIsPatrolling = false;
    bool bIsAttacking;

    AActor* Player;
    AGangAIManager* AIManager;

    // Missing method declarations added
    void HandlePatrolling();
    void HandleChasing();
    void HandleAttacking();
    void HandleReturning();
    bool IsPlayerInChaseRange();
};



