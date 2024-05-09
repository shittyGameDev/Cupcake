// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class CUPCAKE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	float DamageAmount;

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	UStaticMeshComponent* WeaponMesh;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UBoxComponent* WeaponBox;

public:
	// Enable collision
	UFUNCTION()
	void Equip();
	// Disable collision
	UFUNCTION()
	void Unequip();

	UFUNCTION()
	void HideWeapon();
	UFUNCTION()
	void ShowWeapon();
};
