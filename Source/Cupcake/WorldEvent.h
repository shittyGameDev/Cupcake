// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldEvent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UWorldEvent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event Time Trigger")
	int Day;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event Time Trigger")
	int Hour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Event Time Trigger")
	int Minutes;

	UFUNCTION(BlueprintCallable, Category="World Event")
	virtual void Run();
		
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
