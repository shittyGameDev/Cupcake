// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaveLightningActor.generated.h"

class UBoxComponent;
class ADirectionalLight;

UCLASS()
class CUPCAKE_API ACaveLightningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaveLightningActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	ADirectionalLight* DirectionalLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float CaveLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
	float NormalLight;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ToggleLightIntensity(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsCaveLightActive;
	
	

};
