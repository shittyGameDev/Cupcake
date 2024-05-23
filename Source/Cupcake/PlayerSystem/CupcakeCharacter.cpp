// Copyright Epic Games, Inc. All Rights Reserved.

#include "CupcakeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "NewInventoryComponent.h"
#include "Cupcake/UI/BaseHUD.h"
#include "EngineUtils.h"  
#include "SavePlayerProgress.h"
#include "Components/BoxComponent.h"
#include "Cupcake/Actors/AttributeComponent.h"
#include "Cupcake/World/WorldItems/Pickup.h"
#include "Cupcake/World/WorldSystem/TheMapObject.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACupcakeCharacter

ACupcakeCharacter::ACupcakeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Dash
	bIsDashing = false;
	bCanDash = true;
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	PlayerInventory = CreateDefaultSubobject<UNewInventoryComponent>("PlayerInventory");
	PlayerInventory->SetSlotsCapacity(20);

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetCollisionProfileName(TEXT("Trigger"));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ACupcakeCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ACupcakeCharacter::OnOverlapBegin);
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ACupcakeCharacter::OnOverlapEnd);
	PlayerInventory->OnInventoryAdd.AddUObject(this, &ACupcakeCharacter::PlayAddSound);
	//InteractionBox->SetBoxExtent(FVector(0.f, 0.f, 0.f));
	InteractionBox->SetBoxExtent(FVector(100.f, 50.f, 150.f));

	UE_LOG(LogTemp, Warning, TEXT("Inventory slots: %d"), PlayerInventory->GetSlotsCapacity());
	HUD = Cast<ABaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Create Weapon
	if (WeaponBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created weapo"));
		// Spawn the weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());

		// Optionally, attach the weapon to the character
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
		Weapon->Unequip();
		Weapon->HideWeapon();
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player: Missing Weapon Blueprint!"));
	}

	//Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true; 
		PlayerController->bEnableMouseOverEvents = true;
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACupcakeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UpdateFacingDirection();
}

void ACupcakeCharacter::PlayAddSound() const
{
	if (AddSound)
	{
		UGameplayStatics::PlaySound2D(this, AddSound);
	}
}

float ACupcakeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (!bCanTakeDamage) {
		// Actor can't take damage currently
		return 0.0f;
	}
	
	bCanTakeDamage = false; // Disable damage taking
	GetWorldTimerManager().SetTimer(DamageDelayTimerHandle, this, &ACupcakeCharacter::EnableDamageTaking, 0.5f, false);

	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACupcakeCharacter::EnableDamageTaking()
{
	bCanTakeDamage = true;
}

void ACupcakeCharacter::OnDeath_Implementation()
{
	IDamageableInterface::OnDeath_Implementation();

	Destroy();
}

void ACupcakeCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	if (!Weapon) return;

	if (!PlayerInventory->HasItemByID("axe")) return;

	DoAttack();
	Weapon->ShowWeapon();

	// Attack duration
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &ACupcakeCharacter::OnAttackFinished, 1.f, false);
}

void ACupcakeCharacter::OnAttackFinished()
{
	if (Weapon)
	{
		// Unequip weapon
		Weapon->HideWeapon();
	}
}

void ACupcakeCharacter::Dash()
{
	if (!bIsDashing && bCanDash)
	{
		bIsDashing = true;
		bCanDash = false;
		
		FVector DashForce = GetActorForwardVector() * DashImpulseStrength;
		GetCapsuleComponent()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetConstraintMode(EDOFMode::XYPlane);
		GetCapsuleComponent()->BodyInstance.bLockXRotation = true;
		GetCapsuleComponent()->BodyInstance.bLockYRotation = true;
		GetCapsuleComponent()->BodyInstance.bLockZRotation = true;
		GetCapsuleComponent()->AddImpulse(DashForce, NAME_None, true);
		
		GetWorldTimerManager().SetTimer(TimerHandle_Dash, [this]()
		{
			bIsDashing = false;
			
			GetCapsuleComponent()->SetSimulatePhysics(false);
			GetCapsuleComponent()->SetConstraintMode(EDOFMode::None);
			GetCapsuleComponent()->BodyInstance.bLockXRotation = false;
			GetCapsuleComponent()->BodyInstance.bLockYRotation = false;
			GetCapsuleComponent()->BodyInstance.bLockZRotation = false;
			
			GetWorldTimerManager().SetTimer(TimerHandle_Cooldown, [this]()
			{
				bCanDash = true; 
			}, DashCooldown, false);
			
		}, DashDuration, false);
	}
}

void ACupcakeCharacter::UpdateFacingDirection()
{
	if (!GetController())
		return;
	
	if (!PlayerController)
		return;
	
	float MouseX, MouseY;
	if (!PlayerController->GetMousePosition(MouseX, MouseY))
		return;
	
	FVector WorldDirection;
	FVector WorldLocation;
	if (!UGameplayStatics::DeprojectScreenToWorld(PlayerController, FVector2D(MouseX, MouseY), OUT WorldLocation, OUT WorldDirection))
		return;

	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + WorldDirection * 10000.0f; 

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(OUT HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	FVector TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndLocation;

	FVector ToTarget = (TargetPoint - GetActorLocation()).GetSafeNormal();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(ToTarget).Rotator();

	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
	NewRotation.Pitch = 0.0f; 
	NewRotation.Roll = 0.0f; 
	SetActorRotation(NewRotation);
}



void ACupcakeCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
	{
		if (!OverlappedInteractables.Contains(OtherActor))
		{
			OverlappedInteractables.Add(OtherActor);
			FoundInteractable(GetCurrentInteractable());
		}
	}
}

void ACupcakeCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OverlappedInteractables.Contains(OtherActor))
	{
		OverlappedInteractables.Remove(OtherActor);
		FoundInteractable(GetCurrentInteractable());
	}
}

AActor* ACupcakeCharacter::GetCurrentInteractable() const
{
	return OverlappedInteractables.Num() > 0 ? OverlappedInteractables.Last() : nullptr;
}

void ACupcakeCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	// Ensure current interactable ends focus
	if (InteractionData.CurrentInteractable)
	{
		IInteractionInterface* CurrentInteractableInterface = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (CurrentInteractableInterface)
		{
			CurrentInteractableInterface->EndFocus();
		}
	}

	// Update current interactable
	InteractionData.CurrentInteractable = NewInteractable;

	// Begin focus on the new interactable
	if (InteractionData.CurrentInteractable)
	{
		IInteractionInterface* NewInteractableInterface = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (NewInteractableInterface)
		{
			NewInteractableInterface->BeginFocus();

			if (HUD)
			{
				HUD->UpdateInteractionWidget(&NewInteractableInterface->InteractableData);
			}
		}
	}
	else
	{
		if (HUD)
		{
			HUD->HideInteractionWidget();
		}
	}
}



void ACupcakeCharacter::NoInteractableFound()
{
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}
	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ACupcakeCharacter::UpdateInteraction() const
{
	if(TargetInteractable && InteractionData.CurrentInteractable)
	{
		float TotalDuration = TargetInteractable->InteractableData.InteractionDuration;
		float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(TimerHandle_Interaction);
		float Progress = ElapsedTime / TotalDuration;
		Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
		
		if(HUD)
		{
			HUD->UpdateInteractionProgress(Progress);
		}
	}
}

void ACupcakeCharacter::BeginInteract()
{
	if (InteractionData.CurrentInteractable)
	{
		IInteractionInterface* InteractableInterface = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (InteractableInterface)
		{
			InteractableInterface->BeginInteract();

			// If InteractionDuration is nearly zero, call Interact directly
			if (FMath::IsNearlyZero(InteractableInterface->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				// Start a timer if InteractionDuration is valid and above 0.1f
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
												this,
												&ACupcakeCharacter::Interact,
												InteractableInterface->InteractableData.InteractionDuration,
												false);

				GetWorldTimerManager().SetTimer(TimerHandle_ProgressUpdate,
												this,
												&ACupcakeCharacter::UpdateInteraction,
												0.01f,
												true);
			}
		}
	}
}


void ACupcakeCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	GetWorldTimerManager().ClearTimer(TimerHandle_ProgressUpdate);

	if (InteractionData.CurrentInteractable)
	{
		IInteractionInterface* InteractableInterface = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (InteractableInterface)
		{
			InteractableInterface->EndInteract();
		}
	}
}

void ACupcakeCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	GetWorldTimerManager().ClearTimer(TimerHandle_ProgressUpdate);

	if (InteractionData.CurrentInteractable)
	{
		IInteractionInterface* InteractableInterface = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this);
		}
	}
}


void ACupcakeCharacter::UpdateInteractionWidget() const
{
	if(IsValid(TargetInteractable.GetObject()))
	{
		// Whatever interactable object ur looking at, update the HUD
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}



//////////////////////////////////////////////////////////////////////////
// Input

void ACupcakeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		/* Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		*/

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACupcakeCharacter::Move);
		PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACupcakeCharacter::Dash);

		//Interact
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACupcakeCharacter::BeginInteract);
		PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACupcakeCharacter::EndInteract);

		//UI
		PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &ACupcakeCharacter::ToggleMenu);
		PlayerInputComponent->BindAction("ToggleMap", IE_Pressed, this, &ACupcakeCharacter::ToggleMapViaKey);

		PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &ACupcakeCharacter::SaveGame);
		PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ACupcakeCharacter::LoadGame);
		

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACupcakeCharacter::Look);

		// Combat
		PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACupcakeCharacter::Attack);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACupcakeCharacter::DisableMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	bUseControllerRotationYaw = false;
}


void ACupcakeCharacter::EnableMovement()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bUseControllerRotationYaw = true;
}



void ACupcakeCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void ACupcakeCharacter::DropItem(UBaseItem* ItemToDrop, const int32 QuantityToDrop)
{
	if(PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.bNoFail = true;
		//Kollar om itemet man försöker droppa kolliderar med något och flyttar isåfall itemet lite. (Victor)
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 150.f)};
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParameters);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
		PlayDropSound();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void ACupcakeCharacter::RemoveItemFromInventory(UBaseItem* ItemToRemove, const int32 QuantityToRemove)
{
	if(PlayerInventory->FindMatchingItem(ItemToRemove))
	{
		PlayerInventory->RemoveAmountOfItem(ItemToRemove, QuantityToRemove);

		UE_LOG(LogTemp, Warning, TEXT("ItemToRemove: %p was successfully removed!"), ItemToRemove);
	}
}


void ACupcakeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


FVector ACupcakeCharacter::GetMouseForwardDirection()
{
	if (!GetController())
		return FVector::ZeroVector;
	
	if (!PlayerController)
		return FVector::ZeroVector;

	// Retrieve the player controller and get mouse position
	float MouseX, MouseY;
	if (!PlayerController->GetMousePosition(MouseX, MouseY))
		return FVector::ZeroVector;

	FVector WorldDirection;
	FVector WorldLocation;
	if (!UGameplayStatics::DeprojectScreenToWorld(PlayerController, FVector2D(MouseX, MouseY), WorldLocation, WorldDirection))
		return FVector::ZeroVector;

	FVector StartLocation = FollowCamera->GetComponentLocation();
	FVector EndLocation = StartLocation + WorldDirection * 10000.0f; // Extend the direction to a far point

	// Perform a line trace to determine where the mouse is pointing in the world
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

	FVector TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndLocation;
	return (TargetPoint - GetActorLocation()).GetSafeNormal();
}

void ACupcakeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

ATheMapObject* ACupcakeCharacter::FindMapObject()
{
	// Kontrollera att vi har en giltig värld att söka i
	if (GetWorld())
	{
		// Skapa en actor-iterator för ATheMapObject
		for (TActorIterator<ATheMapObject> It(GetWorld()); It; ++It)
		{
			// Returnera den första hittade ATheMapObject-instansen
			return *It;
		}
	}
	return nullptr;  // Returnera nullptr om ingen ATheMapObject hittades
}

void ACupcakeCharacter::ToggleMapViaKey()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleMapVis körs utanpå"));
	ATheMapObject* MapObject = FindMapObject();
	if (MapObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToggleMapVis körs inuti"));
		MapObject->ToggleMapVisibility();
	}
}

//SAVE SYSTEM
//------------------------------------------------------------------

FItemSaveData ConvertToSaveData(UBaseItem* Item)
{
	FItemSaveData SaveData;
	SaveData.ID = Item->ID;
	SaveData.Quantity = Item->Quantity;
	SaveData.ItemType = Item->ItemType;
	SaveData.TextData = Item->TextData;
	SaveData.NumericData = Item->NumericData;
	SaveData.AssetData = Item->AssetData;
	return SaveData;
}

UBaseItem* ConvertToBaseItem(UObject* Outer, const FItemSaveData& SaveData)
{
	UBaseItem* Item = NewObject<UBaseItem>(Outer);
	Item->ID = SaveData.ID;
	Item->Quantity = SaveData.Quantity;
	Item->ItemType = SaveData.ItemType;
	Item->TextData = SaveData.TextData;
	Item->NumericData = SaveData.NumericData;
	Item->AssetData = SaveData.AssetData;
	return Item;
}

void ACupcakeCharacter::SaveGame()
{
	USavePlayerProgress* SaveGameInstance = Cast<USavePlayerProgress>(UGameplayStatics::CreateSaveGameObject(USavePlayerProgress::StaticClass()));

	//Get position and rotation of the player (Victor)
	SaveGameInstance->PlayerPosition = GetActorLocation();
	SaveGameInstance->PlayerRotation = GetActorRotation();

	//Get Inventory and save it (Victor)
	//Can be called in BP
	if (PlayerInventory)
	{
		for (UBaseItem* Item : PlayerInventory->GetInventoryContents())
		{
			SaveGameInstance->InventoryItems.Add(ConvertToSaveData(Item));
		}
	}
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving Game"));
}

void ACupcakeCharacter::LoadGame()
{
	USavePlayerProgress* LoadGameInstance = Cast<USavePlayerProgress>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

	// Set everything we saved in the PlayerSaveSlot. Can be called in BP
	if (LoadGameInstance)
	{
		// Set player position and rotation
		SetActorLocation(LoadGameInstance->PlayerPosition);
		SetActorRotation(LoadGameInstance->PlayerRotation);

		if (PlayerInventory)
		{
			// Clear the inventory first
			PlayerInventory->ClearInventory();

			// Load each item from the save data
			for (const FItemSaveData& SaveData : LoadGameInstance->InventoryItems)
			{
				UBaseItem* NewItem = ConvertToBaseItem(this, SaveData);
				PlayerInventory->HandleLoadItem(NewItem);
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Loaded Save"));
}

