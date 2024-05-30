// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlateSingle.generated.h"

class UNiagaraComponent;
class UBoxComponent;

UCLASS()
class CUPCAKE_API APressurePlateSingle : public AActor
{
	GENERATED_BODY()
	
public:	
	APressurePlateSingle();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TriggerBox")
	UStaticMeshComponent* MushroomTriggerMesh;

	UPROPERTY(EditAnywhere, Category="TriggerBox")
	UBoxComponent* MushroomTrigger;

	UPROPERTY(EditAnywhere, Category="Niagra system")
	UNiagaraComponent* Particles;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAudio();
	UFUNCTION()
	void OnOverlapMushroomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	AActor* Door;

	FVector InitialDoorLocation;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveDuration = 2.0f; // Duration to move down

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector MovementOffset;

	bool bIsMoving = false;
	float ElapsedTime = 0;
};
