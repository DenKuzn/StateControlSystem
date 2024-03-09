// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyExtensionBlueprintFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class MYEXTENSION_API UMyExtensionBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Returns a uniformly distributed random bool*/
	UFUNCTION(BlueprintCallable, Category="Overlaping", meta=(NotBlueprintThreadSafe))
	static void UpdateComponentOverlaps(UPrimitiveComponent* PrimitiveComponent);

	/** Returns a uniformly distributed random bool*/
	UFUNCTION(BlueprintCallable, Category="Navigation", meta=(NotBlueprintThreadSafe))
	static void SetDynamicObstacle(UShapeComponent* PrimitiveComponent, bool bNewDynamicObstacle);
};
