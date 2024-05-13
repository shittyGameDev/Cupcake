// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DonationActor.generated.h"

class AObeliskActor;

UCLASS()
class CUPCAKE_API ADonationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADonationActor();
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void DonationGoalAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	AObeliskActor* ObeliskActor;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MoveDistance = 100.0f;

	// The time over which to move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MoveDuration = 2.0f; 


private:
	// Target location for movement
	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	float MoveTimer;

	UPROPERTY()
	bool bIsMoving;

};
