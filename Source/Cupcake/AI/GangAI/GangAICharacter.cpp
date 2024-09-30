// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAICharacter.h"

#include "GangAIController.h"
#include "GangAIManager.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "../../Items/WeaponBase.h"
#include "Components/AudioComponent.h"
#include "Cupcake/Actors/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AGangAICharacter::AGangAICharacter()
{
    // Enable Tick() function
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationYaw = false;
    
    // Initialize patrol and attack parameters
    PatrolRadius = 500.0f;
    AttackDistance = 200.0f;
    DashDistance = 200.0f;
    ChaseDistance = 1000.0f; // Add this if missing

    // Create attribute component
    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 90.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 200.f;

    // Create and attach Niagara component for visual effects
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeFX"));
    NiagaraComponent->SetupAttachment(RootComponent);

    // Initialize state
    CurrentState = EAIState::Patrolling;
}

// Called when the game starts or when spawned
void AGangAICharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Store initial spawn location
    SpawnLocation = GetActorLocation();

    // Disable Niagara effect initially
    NiagaraComponent->SetActive(false);

    // Get reference to the player pawn
    Player = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("Player pawn is not available"));
    }

    // Find the AI Manager in the world
    TArray<AActor*> FoundManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGangAIManager::StaticClass(), FoundManagers);

    if (FoundManagers.Num() > 0)
    {
        AIManager = Cast<AGangAIManager>(FoundManagers[0]);
        if (AIManager)
        {
            UE_LOG(LogTemp, Warning, TEXT("AI Manager found"));
            // Register this AI character with the manager
            AIManager->RegisterAICharacter(this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AI Manager not found"));
        }
    }

    // Spawn and attach weapon if specified
    if (WeaponBlueprint)
    {
        Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());
        if (Weapon)
        {
            Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
            Weapon->SetOwner(this);
            Weapon->HideWeapon();
            Weapon->Unequip();
        }
    }

    // Start patrolling
    Patrol();
}

// Called every frame
void AGangAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UE_LOG(LogTemp, Warning, TEXT("AGangAICharacter::Tick is being called"));
    if (!Player || !AIManager)
    {
        return;
    }

    switch (CurrentState)
    {
        case EAIState::Patrolling:
            HandlePatrolling();
            break;

        case EAIState::Chasing:
            HandleChasing();
            break;

        case EAIState::Attacking:
            HandleAttacking();
            break;

        case EAIState::Returning:
            HandleReturning();
            break;

        case EAIState::Dead:
            // Dead state logic (if any)
            break;

        default:
            break;
    }

    // Debug visualization: Draw a line to the player
    DrawDebugLine(GetWorld(), GetActorLocation(), Player->GetActorLocation(), FColor::Green, false, -1.f, 0, 1.f);

    // Debug visualization: Draw sphere representing attack distance
    DrawDebugSphere(GetWorld(), GetActorLocation(), AttackDistance, 12, FColor::Red, false, -1.f, 0, 1.f);

    // Debug visualization: Display current state above the AI character
    FString StateText;
    switch (CurrentState)
    {
        case EAIState::Patrolling:
            StateText = TEXT("Patrolling");
            break;
        case EAIState::Chasing:
            StateText = TEXT("Chasing");
            break;
        case EAIState::Attacking:
            StateText = TEXT("Attacking");
            break;
        case EAIState::Returning:
            StateText = TEXT("Returning");
            break;
        case EAIState::Dead:
            StateText = TEXT("Dead");
            break;
        default:
            StateText = TEXT("Unknown");
            break;
    }
    DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 100), StateText, nullptr, FColor::White, 0.f, true);
}

// Handles patrolling behavior
void AGangAICharacter::HandlePatrolling()
{
    if (FVector::Dist(GetActorLocation(), CurrentPatrolPoint) < 100.0f)
    {
        Patrol(); // Get a new patrol point
    }
}

// Handles chasing behavior
void AGangAICharacter::HandleChasing()
{
    float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());

    if (DistanceToPlayer > ChaseDistance)
    {
        // Player is too far; return to patrol
        CurrentState = EAIState::Returning;
        ReturnToPatrol();
    }
    else if (DistanceToPlayer <= AttackDistance && !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_PreAttack) && !bIsAttacking)
    {
        // If within attack range and can attack
        if (AIManager->CanAttack(this))
        {
            InitiateAttack(Player);
        }
    }
    else if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_PreAttack) && !bIsAttacking)
    {
        // Continue chasing the player
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        GetCharacterMovement()->MaxWalkSpeed = 300.f;

        AGangAIController* AIController = Cast<AGangAIController>(GetController());
        if (AIController)
        {
            AIController->MoveToActor(Player, 5.0f, true);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Controller is not of type AGangAIController"));
        }
    }
}

// Handles attacking behavior
void AGangAICharacter::HandleAttacking()
{
    // Attack logic is handled by timers and callbacks
}

// Handles returning to patrol behavior
void AGangAICharacter::HandleReturning()
{
    if (FVector::Dist(GetActorLocation(), SpawnLocation) < 100.0f)
    {
        // Reached spawn location, start patrolling again
        CurrentState = EAIState::Patrolling;
        Patrol();
    }
    else
    {
        AGangAIController* AIController = Cast<AGangAIController>(GetController());
        if (AIController)
        {
            // Move back to spawn location
            AIController->MoveToLocation(SpawnLocation, 5.0f, true);
        }
    }
}

// Checks if the player is within chase range
bool AGangAICharacter::IsPlayerInChaseRange()
{
    float DistanceToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    return DistanceToPlayer <= ChaseDistance;
}

// Starts chasing the target
void AGangAICharacter::StartChasing(AActor* Target)
{
    CurrentState = EAIState::Chasing;
}

// Handles taking damage
float AGangAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // Prevent damage from other AI characters
    if (DamageCauser && DamageCauser->GetOwner() && DamageCauser->GetOwner()->IsA<AGangAICharacter>())
    {
        return 0.f;
    }

    // Proceed with damage
    return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Handles death
void AGangAICharacter::OnDeath_Implementation()
{
    IDamageableInterface::OnDeath_Implementation();
    CurrentState = EAIState::Dead;
    Destroy();
}

// Handles being damaged
void AGangAICharacter::OnDamage_Implementation()
{
    // Change material to indicate hit
    GetMesh()->SetMaterial(0, HitMaterial);

    // Reset material after a delay
    FTimerHandle TimerHandle_ResetMaterial;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetMaterial, [this]()
    {
        GetMesh()->SetMaterial(0, NormalMaterial);
    }, 0.1f, false);

    // If not already chasing or attacking, start chasing
    if (CurrentState != EAIState::Chasing && CurrentState != EAIState::Attacking)
    {
        UE_LOG(LogTemp, Warning, TEXT("Starting to chase the player after being attacked"));

        if (AIManager)
        {
            UE_LOG(LogTemp, Warning, TEXT("Group attack initiated"));
            AIManager->InitiateGroupChase();
        }

        CurrentState = EAIState::Chasing;
    }
}

// Patrols to a random point
void AGangAICharacter::Patrol()
{
    AGangAIController* AIController = Cast<AGangAIController>(GetController());
    if (AIController)
    {
        // Set patrol speed
        GetCharacterMovement()->MaxWalkSpeed = 100.f;

        // Get a random patrol point
        FVector PatrolPoint = GetRandomPatrolPoint();
        CurrentPatrolPoint = PatrolPoint;

        // Move to the patrol point
        AIController->MoveToLocation(PatrolPoint, 1.0f, true, true, false, false, nullptr, true);
    }
}

// Returns to patrol behavior
void AGangAICharacter::ReturnToPatrol()
{
    AGangAIController* AIController = Cast<AGangAIController>(GetController());
    if (AIController)
    {
        // Move back to spawn location
        AIController->MoveToLocation(SpawnLocation, 5.0f, true);
    }
}

// Performs the attack action
void AGangAICharacter::DoAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("Executing attack"));

    if (!Player) return;

    // Calculate direction towards the player
    FVector Direction = Player->GetActorLocation() - GetActorLocation();
    Direction.Normalize();

    // Calculate dash target location
    FVector DashTarget = GetActorLocation() + Direction * DashDistance;

    AGangAIController* AIController = Cast<AGangAIController>(GetController());
    if (AIController)
    {
        // Set movement speed for attack
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        GetCharacterMovement()->MaxWalkSpeed = 800.f;

        // Move towards the dash target
        AIController->MoveToLocation(DashTarget, 1.0f, true);
    }

    // Disable Niagara effect
    NiagaraComponent->SetActive(false);

    // Schedule end of attack
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &AGangAICharacter::OnAttackFinished, 1.0f, false);
}

// Gets a random patrol point within patrol radius
FVector AGangAICharacter::GetRandomPatrolPoint()
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    FNavLocation RandomNavLocation;
    if (NavSys && NavSys->GetRandomPointInNavigableRadius(SpawnLocation, PatrolRadius, RandomNavLocation))
    {
        // Debug visualization: Draw patrol point
        DrawDebugSphere(GetWorld(), RandomNavLocation.Location, 50.f, 8, FColor::Blue, false, 5.f);
        return RandomNavLocation.Location;
    }
    return SpawnLocation;
}

// Handles attack finishing
void AGangAICharacter::OnAttackFinished()
{
    // Reset movement speed
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    bIsAttacking = false;

    // Start cooldown before chasing again
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &AGangAICharacter::EnableChasing, 0.5f, false);
}

// Enables chasing after cooldown
void AGangAICharacter::EnableChasing()
{
    // Resume chasing after cooldown
    CurrentState = EAIState::Chasing;
}

// Initiates attack sequence
void AGangAICharacter::InitiateAttack(AActor* Actor)
{
    if (this)
    {
        bIsAttacking = true;
        UE_LOG(LogTemp, Warning, TEXT("Initiating attack - AI is preparing"));

        // Stop movement
        GetCharacterMovement()->StopMovementImmediately();
        GetCharacterMovement()->DisableMovement();

        // Store target position
        TargetAttackPosition = Actor->GetActorLocation();

        // Activate Niagara effect
        NiagaraComponent->SetActive(true);

        // Change state to Attacking
        CurrentState = EAIState::Attacking;

        // Start attack after delay
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_PreAttack, this, &AGangAICharacter::DoAttack, 1.f, false);
    }
}


