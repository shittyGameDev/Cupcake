// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Pickup.h"
#include "Components/SphereComponent.h"
#include "WeaponBase.generated.h"

UCLASS()
class CUPCAKE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	AActor* OwningActor = nullptr;

	UPROPERTY(EditAnywhere, Category="Weapon Stats")
	float DamageAmount;
	
	UPROPERTY(EditAnywhere, Category="Collision")
	USphereComponent* CollisionComponent;

protected:
	virtual void BeginPlay();
	void Equip(AActor* NewOwner);
	void Unequip();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	void EnableWeapon();
	void DisableWeapon();
};
