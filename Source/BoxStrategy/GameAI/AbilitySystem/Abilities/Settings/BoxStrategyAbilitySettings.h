// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BoxStrategyAbilitySettings.generated.h"

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class BOXSTRATEGY_API UBoxStrategyAbilitySettings : public UObject
{
	GENERATED_BODY()

private:


	UPROPERTY()
	FGameplayTag AbilityTag;

public:


	UFUNCTION(BlueprintCallable)
	void SetAbilityTag(FGameplayTag NewTag);

	UFUNCTION(BlueprintPure)
	FGameplayTag GetAbilityTag();
};
