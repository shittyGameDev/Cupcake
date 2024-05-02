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
#include "Cupcake/Items/Interactable.h"
#include "Cupcake/Items/Item.h"
#include "DrawDebugHelpers.h"
#include "NewInventoryComponent.h"
#include "Cupcake/UI/BaseHUD.h"
#include "Cupcake/World/Pickup.h"
#include "EngineUtils.h"  
#include "Cupcake/TheMapObject.h"
#include "Cupcake/Actors/AttributeComponent.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACupcakeCharacter

ACupcakeCharacter::ACupcakeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
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

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));


	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

float ACupcakeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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
    
	// Attach the weapon to the character, assuming you have a socket named "WeaponSocket" on the character
	if (!Weapon->GetRootComponent()->IsAttachedTo(GetMesh()))
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ik_hand_root"));
		//Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		UE_LOG(LogTemp, Warning, TEXT("Attached"));
	}
	
	Weapon->SetOwner(this);
	Weapon->EnableWeapon(); // Enable the weapon

	// Set a timer to disable the weapon after a short duration, simulating an attack duration
	// Assuming an attack takes 1 second; adjust this duration as needed
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &ACupcakeCharacter::OnAttackFinished, 0.2f, false);
}


void ACupcakeCharacter::OnAttackFinished()
{
	if (Weapon)
	{
		Weapon->DisableWeapon(); // Disable the weapon after the attack is complete
	}
}

void ACupcakeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Inventory slots: %d"), PlayerInventory->GetSlotsCapacity());
	HUD = Cast<ABaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Create Weapon
	if (WeaponBlueprint)
	{
		// Spawn the weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());

		// Optionally, attach the weapon to the character
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));

		FRotator CurrentRotation = Weapon->GetActorRotation();
		CurrentRotation.Yaw += 90.0f;
		Weapon->SetActorRotation(CurrentRotation);
		
		Weapon->DisableWeapon();
	}

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			
		}
	}
	
}

void ACupcakeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}


void ACupcakeCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	//Anledningen till att vi använder oss av initialisering via {} är för att det garanterar typen som vi använder blir
	//Initialiserade korrekt. Annars är koden väldigt basic linetrace skapande. (Victor)
	FVector TraceStart{GetActorLocation()};
	FVector ForwardVector = GetActorForwardVector();
	FVector TraceEnd{TraceStart + (ForwardVector * InteractionCheckDistance)};

	float CapsuleRadius = 100.f;
	float CapsuleHalfHeight = 190.f;

	DrawDebugCapsule(GetWorld(), TraceStart, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Red, false,
	                 1.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;


	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}
	NoInteractableFound();
}

void ACupcakeCharacter::FoundInteractable(AActor* NewInteractable)
{
	if(IsInteracting())
	{
		EndInteract();
	}
	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
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

void ACupcakeCharacter::UpdateInteraction()
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
	// verifiera att ingenting har ändrats med interactable state sedan vi började interaktionen.
	PerformInteractionCheck();
	

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();
			
			// om InteractionDuration nästan är 0 så kallar vi på Interact direkt.
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				// om interactionduration är valid och över 0.1f så startar en timer
				//När timern är klar så kallar vi på interact
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
				                                this,
				                                &ACupcakeCharacter::Interact,
				                                TargetInteractable->InteractableData.InteractionDuration,
				                                false);

				GetWorldTimerManager().SetTimer(TimerHandle_ProgressUpdate,
												this,
												&ACupcakeCharacter::UpdateInteraction,
												.01f,
												true);
				
			}
		}
	}
}

void ACupcakeCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	GetWorldTimerManager().ClearTimer(TimerHandle_ProgressUpdate);
	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ACupcakeCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	GetWorldTimerManager().ClearTimer(TimerHandle_ProgressUpdate);
	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
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

		//Interact
		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACupcakeCharacter::BeginInteract);
		PlayerInputComponent->BindAction("Interact", IE_Released, this, &ACupcakeCharacter::EndInteract);

		//UI
		PlayerInputComponent->BindAction("Hotbar", IE_Pressed, this, &ACupcakeCharacter::HighlightItem);
		PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &ACupcakeCharacter::ToggleMenu);
		PlayerInputComponent->BindAction("ToggleMap", IE_Pressed, this, &ACupcakeCharacter::ToggleMapViaKey);


		/* Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACupcakeCharacter::Look);
		*/

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
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	//bUseControllerRotationYaw = false;
}


void ACupcakeCharacter::EnableMovement()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	//bUseControllerRotationYaw = true;
}

void ACupcakeCharacter::OnRemoveItem()
{
	// Antag att du vill testa att ta bort det första itemet i inventoryt
	if (InventoryComponent->InventoryItems.Num() > 0)
	{
		AItem* ItemToRemove = InventoryComponent->InventoryItems[0];
		InventoryComponent->RemoveItem(ItemToRemove);
	}
}

void ACupcakeCharacter::HighlightItem(FKey KeyPressed)
{
	InventoryComponent->HotbarWidget->HighLightUIItem(KeyPressed);
}


//Temporary function to Interact with items.
void ACupcakeCharacter::OnInteractPressed()
{
	// Define raycast
	FVector Start = GetActorLocation();
	//Assume we want the raycast to be shot in the direction that the player looks
	FVector ForwardVector = GetActorForwardVector();
	FVector End = Start + (ForwardVector * 1000.f); // Justera detta värde för att kontrollera interaktionsavståndet

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Ignore the player

	//Shoot the raycast
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	// Controlling the hit
	if (bHit && HitResult.GetActor() != nullptr)
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(HitResult.GetActor());
		}
	}
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

		const FVector SpawnLocation{GetActorLocation() + (GetActorForwardVector() * 50.f)};
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParameters);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
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
	if (MapObject && MapObject->bCanToggleMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToggleMapVis körs inuti"));
		MapObject->ToggleMapVisibility();
	}
}