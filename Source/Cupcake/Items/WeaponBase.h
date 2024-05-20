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
	void ClearDamagedList();

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	UPrimitiveComponent* Collider;

	UPROPERTY()
	TArray<AActor*> DamagedActors;

public:
	// Enable collision
	UFUNCTION(BlueprintCallable)
	void Equip();
	// Disable collision
	UFUNCTION(BlueprintCallable)
	void Unequip();

	UFUNCTION(BlueprintCallable)
	void HideWeapon();
	UFUNCTION(BlueprintCallable)
	void ShowWeapon();
};
