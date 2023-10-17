// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyAttributeSet.h"

#include "BoxStrategy/GameAI/AbilitySystem/BoxStrategyAbilitySystemComponent.h"

UBoxStrategyAttributeSet::UBoxStrategyAttributeSet()
{
}

UWorld* UBoxStrategyAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UBoxStrategyAbilitySystemComponent* UBoxStrategyAttributeSet::GetBoxStrategyAbilitySystemComponent() const
{
	return Cast<UBoxStrategyAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
