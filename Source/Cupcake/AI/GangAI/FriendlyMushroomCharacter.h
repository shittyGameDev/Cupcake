#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "FriendlyMushroomCharacter.generated.h"

class UBaseItem;

UCLASS()
class CUPCAKE_API AFriendlyMushroomCharacter : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AFriendlyMushroomCharacter();

	UFUNCTION()
	void MoveToLocation(const FVector& NewLocation);

	UPROPERTY(EditAnywhere, Category="Movement")
	TArray<AActor*> MoveLocations;

protected:
	virtual void BeginPlay() override;
	void MoveTrigger();
	void MoveToNextLocation();

	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(EditInstanceOnly, Category= "Craft | Item Initialization")
	int32 ItemQuantity;
	
	UPROPERTY(VisibleAnywhere, Category= "Craft | Item Reference")
	UBaseItem* ItemReference;
	
	UPROPERTY(EditInstanceOnly, Category="Interact Text")
	FInteractableData InstanceInteractableData;

public:
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ACupcakeCharacter* PlayerCharacter) override;

	void InitMushroomItem(const TSubclassOf<UBaseItem> BaseClass, const int32 InQuantity);


protected:
	FVector Destination;
};
