// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAICharacter.h"

#include "GangAIController.h"
#include "GangAIManager.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "Cupcake/EnemyCharacter.h"
#include "Cupcake/WeaponBase.h"
#include "Cupcake/Actors/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AGangAICharacter::AGangAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PatrolRadius = 500.0f;
	AttackDistance = 200.0f;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	bIsChasing = false;
	bIsAttacking = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeFX"));
	NiagaraComponent->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AGangAICharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnLocation = GetActorLocation();
	NiagaraComponent->SetActive(false);
	
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGangAIManager::StaticClass(), FoundManagers);
	if (WeaponBlueprint)
	{
		// Spawn the weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());

		// Optionally, attach the weapon to the character
		//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));

		Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		Weapon->HideWeapon();
	}
	if (FoundManagers.Num() > 0)
	{
		AIManager = Cast<AGangAIManager>(FoundManagers[0]);
		if (AIManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("AImanager hittades"));
			AIManager->RegisterAICharacter(this);
		}
	}
	Patrol();
}

// Called every frame
void AGangAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if(!Player)
	{
		return;
	}
	float DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), GetActorLocation());

	if (bIsChasing && !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Cooldown))
	{
		if (DistanceToPlayer > ChaseDistance)
		{
			ReturnToPatrol();
		}
		else if (DistanceToPlayer <= AttackDistance && !GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_PreAttack) && !bIsAttacking)
		{
			// Log the attack initiation and disable movement
			bIsAttacking = true;
			UE_LOG(LogTemp, Warning, TEXT("Preparing to attack - AI is standing still"));
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();
			TargetAttackPosition = Player->GetActorLocation();
			NiagaraComponent->SetActive(true);
			// Set a timer to call DoAttack after a 0.5 second delay
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_PreAttack, this, &AGangAICharacter::DoAttack, 1.f, false);
		}
		else if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_PreAttack))
		{
			// If not within attack range and not preparing an attack, continue chasing
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			Cast<AGangAIController>(GetController())->MoveToActor(Player, 5.0f, true);
		}
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		if (FVector::Dist(CurrentLocation, CurrentPatrolPoint) < 100.0f)
		{
			Patrol(); // Update to a new patrol point
		}
	}
}

void AGangAICharacter::StartChasing(AActor* Target)
{
	bIsChasing = true;
	
}

float AGangAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AGangAICharacter::OnDeath_Implementation()
{
	IDamageableInterface::OnDeath_Implementation();
	Destroy();
}

void AGangAICharacter::OnDamage_Implementation()
{
	if (!bIsChasing)
	{
		bIsChasing = true;
		UE_LOG(LogTemp, Warning, TEXT("Börja jaga spelaren"));
		if (AIManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("GRUPPEN ATTACKERA"));
			AIManager->InitiateGroupChase();  
		}
	}
}

void AGangAICharacter::Patrol()
{
	if (!bIsChasing)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			GetCharacterMovement()->MaxWalkSpeed = 100.f;
			FVector PatrolPoint = GetRandomPatrolPoint();
			CurrentPatrolPoint = PatrolPoint;
			AIController->MoveToLocation(PatrolPoint, 1.0f, true, true, false, true, nullptr, true);
		}
	}
}

void AGangAICharacter::ReturnToPatrol()
{
	bIsChasing = false;
	AGangAIController* AIController = Cast<AGangAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(SpawnLocation, 5.0f, true);
		Patrol();
	}
}

void AGangAICharacter::DoAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attacking - Dashing towards the target"));
	AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	AAIController* AIController = Cast<AGangAIController>(GetController());
	// Enable movement and dash towards the player
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	AIController->MoveToLocation(TargetAttackPosition, 1.0f, true);
	NiagaraComponent->SetActive(false);
	// Assuming the weapon should be enabled
	if (Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is not null"));
		Weapon->SetOwner(this);
		Weapon->ShowWeapon();
	}

	// Set a timer to stop the attack
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackFinished, this, &AGangAICharacter::OnAttackFinished, 1.0f, false);
}

FVector AGangAICharacter::GetRandomPatrolPoint()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation RandomNavLocation;
	if (NavSys && NavSys->GetRandomPointInNavigableRadius(SpawnLocation, PatrolRadius, RandomNavLocation))
	{
		return RandomNavLocation.Location;
	}
	return SpawnLocation;
}

void AGangAICharacter::OnAttackFinished()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	bIsAttacking = false;
	if (Weapon)
	{
		Weapon->HideWeapon();
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &AGangAICharacter::EnableChasing, 2.0f, false);
}

void AGangAICharacter::EnableChasing()
{
	bIsChasing = true;
}


