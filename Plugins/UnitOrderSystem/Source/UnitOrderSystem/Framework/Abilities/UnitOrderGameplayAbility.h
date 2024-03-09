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

public:

	UFUNCTION(BlueprintPure, Category = "UnitOrder")
	const FGameplayTag& GetMyUnitSubtypeTag() const;

	void SetAbilityTag(FGameplayTag NewAbilityTag);

	FGameplayTag GetAbilityTag() const;

	UFUNCTION(BlueprintPure, Category = "UnitOrder")
	UUnitOrderAbilitySystemComponent* GetUnitOrderAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "UnitOrder")
	const UUnitOrderAbilitySettings* GetMyAbilitySettings() const;
};
