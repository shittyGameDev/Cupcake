// Fill out your copyright notice in the Description page of Project Settings.


#include "TheMapObject.h"

#include "Kismet/GameplayStatics.h"
#include "../../PlayerSystem/CupcakeCharacter.h"

// Sets default values
ATheMapObject::ATheMapObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(100.f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATheMapObject::OnOverlapBegin);
}

void ATheMapObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACupcakeCharacter::StaticClass()))
	{
		bCanToggleMap = true;
	}
	
}

void ATheMapObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (OtherActor && OtherActor->IsA(ACupcakeCharacter::StaticClass()))
	{
		bCanToggleMap = false;
	}
}

// Called when the game starts or when spawned
void ATheMapObject::BeginPlay()
{
	Super::BeginPlay();
	if (!MapWidget && GetWorld())
	{
		MapWidget = CreateWidget<UUserWidget>(GetWorld(), MapWidgetClass);
		if (MapWidget)
		{
			MapWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Widget created successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create widget"));
		}
	}
	MapWidget->SetVisibility(ESlateVisibility::Collapsed);
	
}

void ATheMapObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap has occurred"));
	if (OtherActor && OtherActor->IsA(ACupcakeCharacter::StaticClass()))
	{
		ToggleMapVisibility();
	}
}


void ATheMapObject::ToggleMapVisibility()
{
	if (MapWidget)
	{
		bool bIsVisible = MapWidget->IsVisible();
		ACupcakeCharacter* Player = Cast<ACupcakeCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (bIsVisible)
		{
			MapWidget->SetVisibility(ESlateVisibility::Hidden);
			OnMapVisibilityChanged(false);
		}
		else
		{
			MapWidget->SetVisibility(ESlateVisibility::Visible);
			OnMapVisibilityChanged(true);
		}
	}
}





