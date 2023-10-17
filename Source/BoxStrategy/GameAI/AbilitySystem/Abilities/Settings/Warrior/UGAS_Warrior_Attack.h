// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BoxStrategyAbilitySettings.h"
#include "UGAS_Warrior_Attack.generated.h"

/**
 * 
 */
UCLASS()
class BOXSTRATEGY_API UUGAS_Warrior_Attack : public UBoxStrategyAbilitySettings
{
	GENERATED_BODY()

private:

	UPROPERTY()
	AActor* TargetActor = nullptr;

	bool bNeedChaseTarget = false;

public:

	UFUNCTION(BlueprintCallable, Category = "Ability Settings")
	void SetAbilitySettings(AActor* NewTargetActor, bool NeedChaseTarget);

	AActor* GetTargetActor() const { return TargetActor; }

	bool GetNeedChaseTarget() const { return bNeedChaseTarget; }
};
