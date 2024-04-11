// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Item.generated.h"


class ACupcakeCharacter;

UCLASS()
class CUPCAKE_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();


	//Different variables associated with every of item. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemThumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 Quantity;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	int32 ItemTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	USphereComponent* SphereCollisionComponent;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Function to handle the overlap event. 
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	virtual void Interact_Implementation();
};
