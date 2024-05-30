// AIGroupManager.h
#pragma once
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GangAIManager.generated.h"


UCLASS(Blueprintable)
class CUPCAKE_API AGangAIManager : public AActor
{
	GENERATED_BODY()

public:
	AGangAIManager();
	
	void InitiateGroupChase();

	UPROPERTY(BlueprintReadWrite, Category="Enemies")
	TArray<class AGangAICharacter*> RegisteredAICharacters;

	void RegisterAICharacter(AGangAICharacter* AICharacter);

	bool CanAttack(AGangAICharacter* RequestingAI);

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	//UBillboardComponent* BillboardComponent;
};