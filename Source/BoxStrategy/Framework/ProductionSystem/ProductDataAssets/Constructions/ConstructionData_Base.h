// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/ProductionSystem/ProductDataAssets/BoxStrategyProductData_Base.h"
#include "ConstructionData_Base.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UConstructionData_Base : public UBoxStrategyProductData_Base
{
	GENERATED_BODY()

public:

	// Building Blueprint for spawn when player have chosen place for building
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Construction")
	TSoftClassPtr<ABoxStrategyBuilding> ConstructionBlueprintClass = nullptr;
};
