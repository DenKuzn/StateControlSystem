// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "BoxStrategyGameplayAbility.generated.h"

class UBoxStrategyAbilitySettings;
class UBoxStrategyAbilitySystemComponent;
/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UBoxStrategyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UBoxStrategyGameplayAbility(const FObjectInitializer& ObjectInitializer);

private:

	// У абилок есть свой тег, который используется для поиска в AbilitySystemComponent. Лучше использовать его, не смотря на то, что он является массивом.
	//FGameplayTag AbilityTag;

public:

	void SetAbilityTag(FGameplayTag NewAbilityTag);

	FGameplayTag GetAbilityTag() const;

	UFUNCTION(BlueprintPure, Category = "BoxStrategy")
	UBoxStrategyAbilitySystemComponent* GetBoxStrategyAbilitySystemComponent();

	UFUNCTION(BlueprintPure, Category = "BoxStrategy")
	const UBoxStrategyAbilitySettings* GetMyAbilitySettings();
};
