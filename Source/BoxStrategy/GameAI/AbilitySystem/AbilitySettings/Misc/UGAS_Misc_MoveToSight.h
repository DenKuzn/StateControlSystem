// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitOrderSystem/Framework/Abilities/Settings/UnitOrderAbilitySettings.h"
#include "UGAS_Misc_MoveToSight.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UUGAS_Misc_MoveToSight : public UUnitOrderAbilitySettings
{
	GENERATED_BODY()

private:

	AActor* TargetActor = nullptr;

	float MinimalDistance = -1.f;

public:

	AActor* GetTargetActor() const { return TargetActor; };

	void SetTargetActor(AActor* NewTargetActor) { TargetActor = NewTargetActor; };

	void SetMinimalDistance(float NewMinimalDistance) { MinimalDistance = NewMinimalDistance; };

	float GetMinimalDistance() const { return MinimalDistance; };
};
