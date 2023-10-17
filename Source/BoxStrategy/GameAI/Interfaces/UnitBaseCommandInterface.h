// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitBaseCommandInterface.generated.h"


class UGameAIUnitInteractComponent;

/**
 *
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUnitBaseCommandInterface : public UInterface
{
	GENERATED_BODY()
};


class BOXSTRATEGY_API IUnitBaseCommandInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitBaseCommandInterface")
	void Order_MoveToLocation(FVector ToLocation);


	// We cannot change type because we need to understand what we are interacting with.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UnitBaseCommandInterface")
	void Order_InteractWithObject(UGameAIUnitInteractComponent* InteractiveComponent);

};