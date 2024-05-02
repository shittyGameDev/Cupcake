// AIGroupManager.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GangAIManager.generated.h"

UCLASS()
class CUPCAKE_API UGangAIManager : public UObject
{
	GENERATED_BODY()

public:
	void InitiateGroupChase();

	TArray<class AGangAICharacter*> RegisteredAICharacters;

	void RegisterAICharacter(AGangAICharacter* AICharacter);
};