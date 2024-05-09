#include "DamageableInterface.h"

#include "AttributeComponent.h"

float IDamageableInterface::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!Attributes)
	{
		UE_LOG(LogTemp, Warning, TEXT("IDamageableInterface::TakeDamage, Attributes is NULL"));
		return 0.f;
	}
	Attributes->ReceiveDamage(DamageAmount);
	Execute_OnDamage(Attributes->GetOwner());
	return DamageAmount;
}