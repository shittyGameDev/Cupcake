// Fill out your copyright notice in the Description page of Project Settings.


#include "GangAICharacter.h"

#include "GangAIController.h"
#include "GangAIManager.h"
#include "NavigationSystem.h"
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
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	bIsChasing = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

// Called when the game starts or when spawned
void AGangAICharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnLocation = GetActorLocation();
	
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGangAIManager::StaticClass(), FoundManagers);

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
	if (bIsChasing)
	{
		AActor* Player = UGameplayStatics::GetPlayerPawn(this, 0);
		float DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), GetActorLocation());
        
		if (DistanceToPlayer > ChaseDistance)
		{
			ReturnToPatrol();
		}
		else
		{
			AGangAIController* AIController = Cast<AGangAIController>(GetController());
			if (AIController)
			{
				GetCharacterMovement()->MaxWalkSpeed = 300.f;
				AIController->MoveToActor(Player, 5.0f, true);
			}
		}
	}
	else
	{
		// Check if close to current patrol point, if so, get a new one
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



