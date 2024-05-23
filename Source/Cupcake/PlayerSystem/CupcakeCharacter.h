// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Items/WeaponBase.h"
#include "Cupcake/Actors/DamageableInterface.h"
#include "Cupcake/Items/InteractionInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CupcakeCharacter.generated.h"

class UNiagaraComponent;
class UBoxComponent;
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
	
	//Look Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ACupcakeCharacter();
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                 AActor* DamageCauser);

	UFUNCTION()
	virtual void OnDeath_Implementation();

	UFUNCTION()
	void Attack();

	UFUNCTION(BlueprintImplementableEvent)
	void DoAttack();

	UFUNCTION()
	void OnAttackFinished();

	UFUNCTION(BlueprintCallable)
	void Dash();

	UFUNCTION(BlueprintCallable)
	void EnableMovement();

	UFUNCTION(BlueprintCallable)
	void DisableMovement();

	UFUNCTION()
	void ToggleMapViaKey();

	UPROPERTY(EditAnywhere, Category = "Interaction")
	UBoxComponent* InteractionBox;

	UFUNCTION()
	ATheMapObject* FindMapObject();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	FORCEINLINE UNewInventoryComponent* GetInventory() const { return PlayerInventory; }

	void UpdateInteractionWidget() const;
	
	UPROPERTY(Blueprintable, BlueprintGetter=GetWeapon)
	AWeaponBase* Weapon;

	UFUNCTION(BlueprintGetter)
	AWeaponBase* GetWeapon() const { return Weapon; }
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeaponBase> WeaponBlueprint;
	
	FTimerHandle TimerHandle_AttackFinished;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

protected:

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* AddSound;
	
	UFUNCTION()
	void PlayAddSound() const;
	
	UPROPERTY()
	ABaseHUD* HUD;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	FVector GetMouseForwardDirection();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	AActor* GetCurrentInteractable() const;

	UPROPERTY(VisibleAnywhere, Category="Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character | Inventory")
	UNewInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;
	FTimerHandle TimerHandle_ProgressUpdate;

	FInteractionData InteractionData;

	//void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void UpdateInteraction() const;
	void BeginInteract();
	void EndInteract();
	void Interact();

public:
	
	virtual void Tick(float DeltaSeconds) override;
	void UpdateFacingDirection();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);};

	void ToggleMenu();

	void DropItem(UBaseItem* ItemToDrop, const int32 QuantityToDrop);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDropSound();

	void RemoveItemFromInventory(UBaseItem* ItemToRemove, const int32 QuantityToRemove);

private:
	FTimerHandle DamageDelayTimerHandle;
	void EnableDamageTaking();
	bool bCanTakeDamage = true;
	FTimerHandle TimerHandle_Dash;
	FTimerHandle TimerHandle_Cooldown;
	bool bIsDashing;
	bool bCanDash; 
	float DashImpulseStrength = 2000.f;
	float DashDuration = 0.15f; 
	float DashCooldown = 1.0f;

	UPROPERTY()
	TArray<AActor*> OverlappedInteractables;
};

