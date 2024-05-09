#pragma once

#include "CoreMinimal.h"
#include "DamageableInterface.generated.h"

class UAttributeComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class CUPCAKE_API IDamageableInterface
{
	GENERATED_BODY()
	
public:
	IDamageableInterface() {}
	
	explicit IDamageableInterface(UAttributeComponent* InAttributes) : Attributes(InAttributes) {}
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void ResetDamageCooldown();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDeath();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDamage();
	
	UAttributeComponent* Attributes;
};