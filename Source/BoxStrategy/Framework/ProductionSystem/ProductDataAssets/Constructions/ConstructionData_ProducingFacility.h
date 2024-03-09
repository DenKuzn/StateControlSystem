// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstructionData_Base.h"
#include "ConstructionData_ProducingFacility.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UConstructionData_ProducingFacility : public UConstructionData_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Facility")
	TArray<UBoxStrategyProductData_Base*> ProducedProducts;
};
