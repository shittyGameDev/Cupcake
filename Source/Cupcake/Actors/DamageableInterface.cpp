#include "DamageableInterface.h"

#include "AttributeComponent.h"

float IDamageableInterface::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("Damage Causer: %s"), *DamageCauser->GetOwner()->GetActorLabel());
	UE_LOG(LogTemp, Display, TEXT("Damage Taker: %s"), *Cast<AActor>(this)->GetActorLabel());
	if (DamageCauser->GetOwner()->GetActorLabel().Equals(Cast<AActor>(this)->GetActorLabel())) return 0.f;
	if (!Attributes)
	{
		UE_LOG(LogTemp, Warning, TEXT("IDamageableInterface::TakeDamage, Attributes is NULL"));
		return 0.f;
	}
	Attributes->ReceiveDamage(DamageAmount);
	Execute_OnDamage(Attributes->GetOwner());
	return DamageAmount;
}