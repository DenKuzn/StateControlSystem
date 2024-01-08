// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "UnitOrderGameplayAbility.generated.h"

class UUnitOrderAbilitySettings;
class UUnitOrderAbilitySystemComponent;
/**
 *
 */
UCLASS()
class UNITORDERSYSTEM_API UUnitOrderGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UUnitOrderGameplayAbility(const FObjectInitializer& ObjectInitializer);

private:

	// У абилок есть свой тег, который используется для поиска в AbilitySystemComponent. Лучше использовать его, не смотря на то, что он является массивом.
	//FGameplayTag AbilityTag;

public:

	void SetAbilityTag(FGameplayTag NewAbilityTag);

	FGameplayTag GetAbilityTag() const;

	UFUNCTION(BlueprintPure, Category = "UnitOrder")
	UUnitOrderAbilitySystemComponent* GetUnitOrderAbilitySystemComponent();

	UFUNCTION(BlueprintPure, Category = "UnitOrder")
	const UUnitOrderAbilitySettings* GetMyAbilitySettings();
};
