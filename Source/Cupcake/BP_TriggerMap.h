// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BP_TriggerMap.generated.h"

class UTheMapHandler;

UCLASS()
class CUPCAKE_API ABP_TriggerMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABP_TriggerMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
	UUserWidget* ActivePickupNotificationWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UPROPERTY(EditAnywhere)
	//class UUserWidget* LinkedWidget;

	UPROPERTY(EditAnywhere)
	UTheMapHandler* TheMapHandler;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PickupNotificationWidgetClass;
	
	UPROPERTY(EditInstanceOnly)
	int32 id;
};
