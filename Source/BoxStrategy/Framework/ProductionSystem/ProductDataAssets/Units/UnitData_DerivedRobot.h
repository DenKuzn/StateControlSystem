// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitData_Base.h"
#include "UnitData_DerivedRobot.generated.h"

/**
 *
 */
UCLASS(meta = (DisplayName = "Production Unit Data Derived Robot"))
class BOXSTRATEGY_API UUnitData_DerivedRobot : public UUnitData_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitData")
	FGameplayTag RequiredUnitForConstruction;
};
