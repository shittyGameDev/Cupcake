#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EItemType : uint8
{
	Wood UMETA(DisplayName = "Wood"),
	Stone UMETA(DisplayName = "Stone"),
	Key UMETA(DisplayName = "Key"),
	Quest UMETA(DisplayName = "Quest"),
	Misc UMETA(DisplayName = "Misc")
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	UPROPERTY(EditAnywhere)
	FText UsageText;
	
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category= "Item ID")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category= "Item Type")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category= "Item Text Data")
	FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category= "Item Numeric Data")
	FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category= "Item Asset Data")
	FItemAssetData AssetData;
	
};
