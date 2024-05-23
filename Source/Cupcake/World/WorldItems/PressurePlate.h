// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

class UBoxComponent;

UCLASS()
class CUPCAKE_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APressurePlate();
	
	UPROPERTY(EditAnywhere, Category="TriggerBox")
	UStaticMeshComponent* MushroomTriggerMesh;

	UPROPERTY(EditAnywhere, Category="TriggerBox")
	UBoxComponent* MushroomTrigger;
	
	UPROPERTY(EditAnywhere, Category="TriggerBox")
	UBoxComponent* PlayerTrigger;

	UPROPERTY(EditAnywhere, Category="TriggerBox")
	UStaticMeshComponent* PlayerTriggerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TriggerBox")
	UStaticMeshComponent* DoorMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapMushroomBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapMushroomEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlapPlayerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapPlayerEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerTriggered = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsMushroomTriggered = false;

	UFUNCTION(BlueprintImplementableEvent)
	void UnlockDoor();
};
