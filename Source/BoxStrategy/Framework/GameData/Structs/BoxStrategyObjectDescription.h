// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoxStrategyObjectDescription.generated.h"

/** Базовая структура для описания объектов.
 * 
 */

USTRUCT( BlueprintType )
struct FBoxStrategyObjectDescription
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText MainName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText ShortDescription;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FText FullDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> ItemImage = nullptr;
};