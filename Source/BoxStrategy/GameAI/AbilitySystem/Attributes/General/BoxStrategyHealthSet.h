// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"

#include "../BoxStrategyAttributeSet.h"
#include "BoxStrategyHealthSet.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UBoxStrategyHealthSet : public UBoxStrategyAttributeSet
{
	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(UBoxStrategyHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UBoxStrategyHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBoxStrategyHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UBoxStrategyHealthSet, Damage);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FBoxStrategyAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FBoxStrategyAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FBoxStrategyAttributeEvent OnOutOfHealth;

private:

	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "BoxStrategy|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, Category = "BoxStrategy|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	/*
	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the health before any changes
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	*/

	// -------------------------------------------------------------------
	//	Meta Attribute (please keep attributes that aren't 'stateful' below
	// -------------------------------------------------------------------

	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category="BoxStrategy|Health", Meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="BoxStrategy|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
