// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Cupcake/WeaponBase.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CupcakeCharacter.generated.h"

class UBaseItem;
class UNewInventoryComponent;
class ABaseHUD;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()
	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};
	UPROPERTY()
	AActor* CurrentInteractable;
	UPROPERTY()
	float LastInteractionCheckTime;
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACupcakeCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerController* PlayerController;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	*/

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UInventoryComponent* InventoryComponent;


	//Look Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ACupcakeCharacter();
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                 AActor* DamageCauser);

	UFUNCTION()
	virtual void OnDeath_Implementation();

	void Attack();

	UFUNCTION()
	void OnAttackFinished();

	UFUNCTION()
	void EnableMovement();

	UFUNCTION()
	void DisableMovement();

	UFUNCTION()
	void ToggleMapViaKey();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	USphereComponent* InteractionSphere;

	UFUNCTION()
	ATheMapObject* FindMapObject();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE UNewInventoryComponent* GetInventory() const { return PlayerInventory; }

	void UpdateInteractionWidget() const;
	
	UPROPERTY()
	AWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;
	
	FTimerHandle TimerHandle_AttackFinished;

protected:
	UPROPERTY()
	ABaseHUD* HUD;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Handler for when "Interact" is pressed
	UFUNCTION()
	void OnInteractPressed();

	UFUNCTION()
	void OnRemoveItem();

	UFUNCTION()
	void HighlightItem(const FKey KeyPressed);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category="Character | Inventory")
	UNewInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;
	FTimerHandle TimerHandle_ProgressUpdate;

	FInteractionData InteractionData;

	//void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void UpdateInteraction();
	void BeginInteract();
	void EndInteract();
	void Interact();

public:
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);};

	void ToggleMenu();

	void DropItem(UBaseItem* ItemToDrop, const int32 QuantityToDrop);

	void RemoveItemFromInventory(UBaseItem* ItemToRemove, const int32 QuantityToRemove);
};

