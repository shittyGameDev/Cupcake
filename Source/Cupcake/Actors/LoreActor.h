// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "LoreActor.generated.h"

UCLASS()
class CUPCAKE_API ALoreActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoreActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditInstanceOnly, Category="Test Actor")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditAnywhere, Category="Materials")
	TArray<UMaterialInterface*> OutlineMaterials;

	UFUNCTION(BlueprintImplementableEvent)
	void SetOutline();

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveOutline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
};
