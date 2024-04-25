#include "DamagableComponent.h"

#include "AttributeComponent.h"

UDamagableComponent::UDamagableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>("AttributeComponent");
}

float UDamagableComponent::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	AttributeComponent->ReceiveDamage(DamageAmount);
	OnDamage();
	return DamageAmount;
}

void UDamagableComponent::OnDamage_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s took damage"), *GetOwner()->GetActorLabel());
}

void UDamagableComponent::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("%s died"), *GetOwner()->GetActorLabel());
	if (AActor* Owner = GetOwner())
	{
		Owner->Destroy();
	}
}