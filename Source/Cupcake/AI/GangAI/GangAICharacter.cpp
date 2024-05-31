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
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PatrolRadius = 500.0f;
	AttackDistance = 200.0f;
	DashDistance = 200.0f;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	bIsChasing = false;
	bIsAttacking = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
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
	Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Player pawn is not available"));
	}
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGangAIManager::StaticClass(), FoundManagers);
	
	if (WeaponBlueprint)
	{
		// Spawn the weapon
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponBlueprint, GetActorLocation(), GetActorRotation());

		// Optionally, attach the weapon to the character
		//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));

		Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		Weapon->SetOwner(this);

		Weapon->HideWeapon();
		Weapon->Unequip();
	}
	if (FoundManagers.Num() > 0)
	{
		AIManager = Cast<AGangAIManager>(FoundManagers[0]);
		if (AIManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("AImanager hittades"));
			AIManager->RegisterAICharacter(this);
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("AImanager hittas inte"));
		}
	}
	Patrol();
}

// Called every frame
void AGangAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!Player)
	{
		return;
	}

	if (!AIManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIManager is not valid"));
		return; // Early return if AIManager is not valid
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
			if (AIManager && AIManager->CanAttack(this))
			{
				InitiateAttack(Player);
			}
		}
		else if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_PreAttack) && !bIsAttacking)
		{
			// If not within attack range and not preparing an attack, continue chasing
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			if (Cast<AGangAIController>(GetController()))
			{
				Cast<AGangAIController>(GetController())->MoveToActor(Player, 5.0f, true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Controller is not of type AGangAIController"));
			}
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
	// Cast the DamageCauser to AGangAICharacter
	AGangAICharacter* CausingCharacter = Cast<AGangAICharacter>(DamageCauser->Owner);
    
	// If the cast is successful, prevent damage between AI characters
	if (!CausingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Gör  skada"));
		return IDamageableInterface::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	return 0.f;
}

void AGangAICharacter::OnDeath_Implementation()
{
	IDamageableInterface::OnDeath_Implementation();
	Destroy();
}

void AGangAICharacter::OnDamage_Implementation()
{
	GetMesh()->SetMaterial(0, HitMaterial);
	FTimerHandle TimerHandle_ResetMaterial;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetMaterial, [this]()
	{
		GetMesh()->SetMaterial(0, NormalMaterial);
	}, 0.1f, false);
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

			// Calculate the direction to the patrol point
			FVector Direction = PatrolPoint - GetActorLocation();
			FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

			// Smoothly interpolate rotation towards the patrol point
			FRotator CurrentRotation = GetActorRotation();
			FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 2.0f); // Adjust the interpolation speed as needed

			SetActorRotation(InterpolatedRotation);

			// Move to the patrol point
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
	UE_LOG(LogTemp, Warning, TEXT("Preparing to dash towards the target"));

	
	if (!Player) return;

	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Normalize();  
	
	FVector DashTarget = GetActorLocation() + Direction * DashDistance;

	AAIController* AIController = Cast<AGangAIController>(GetController());
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->MaxWalkSpeed = 800.f; 
	AIController->MoveToLocation(DashTarget, 1.0f, true); 
	
	NiagaraComponent->SetActive(false);
	
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
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Cooldown, this, &AGangAICharacter::EnableChasing, .5f, false);
}

void AGangAICharacter::EnableChasing()
{
	bIsChasing = true;
}

void AGangAICharacter::InitiateAttack(AActor* Actor)
{

	if(this != nullptr)
	{	bIsAttacking = true;
		UE_LOG(LogTemp, Warning, TEXT("Preparing to attack - AI is standing still"));
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
		TargetAttackPosition = Actor->GetActorLocation();
		NiagaraComponent->SetActive(true);
		StartAttack();
	
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PreAttack, this, &AGangAICharacter::DoAttack, 1.f, false);
	}

}



