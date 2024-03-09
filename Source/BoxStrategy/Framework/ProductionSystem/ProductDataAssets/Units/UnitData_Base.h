// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/ProductionSystem/ProductDataAssets/BoxStrategyProductData_Base.h"

#include "UnitData_Base.generated.h"

/** For all Subtypes units
 *
 */
UCLASS(meta = (DisplayName = "Production Unit Data Base"))
class BOXSTRATEGY_API UUnitData_Base : public UBoxStrategyProductData_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData")
	FGameplayTag UnitSubtypeTag;

	// Building Blueprint for spawn when player have chosen place for building
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData")
	TSoftClassPtr<APawn> UnitBlueprintClass = nullptr;



};
