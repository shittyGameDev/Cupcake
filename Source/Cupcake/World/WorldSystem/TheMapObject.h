#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TheMapObject.generated.h"

/**
 * Klass som representerar objektet som aktiverar kartwidgeten när spelaren närmar sig och interagerar.
 */
UCLASS(Blueprintable)
class CUPCAKE_API ATheMapObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATheMapObject();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMapVisibilityChanged);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Map")
	FOnMapVisibilityChanged OnMapOpened;

	UFUNCTION(BlueprintImplementableEvent)
	void OnMapOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void OnMapClosedWidget();
	// Event när kartan göms
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Map")
	FOnMapVisibilityChanged OnMapClosed;
	// Widget to show when the map object is interacted with
	UPROPERTY(Transient, BlueprintReadWrite)
	UUserWidget* MapWidget;

	// Ny medlemsvariabel för att spåra om spelaren kan interagera
	bool bCanToggleMap = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsVisibles;
    
	// Getter metod för bIsVisibles
	UFUNCTION(BlueprintCallable, Category="State")
	bool GetIsVisibles() const;

	FTimerHandle VisibilityTimerHandle;

	// Hantering av spelarinteraktion
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Sphere component to handle overlap events
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	// Static mesh to represent the object visually
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;



	// Widget class to create from
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MapWidgetClass;

public: 
	// Function to handle the beginning of an overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Function to toggle the visibility of the map widget
	UFUNCTION(BlueprintCallable)
	void ToggleMapVisibility();

	UFUNCTION(BlueprintImplementableEvent, Category = "Map")
	void OnMapVisibilityChanged(bool bIsVisible);

	void HideMapWidget();
};