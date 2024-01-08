// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderAttributeSet.h"

#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"

UUnitOrderAttributeSet::UUnitOrderAttributeSet()
{
}

UWorld* UUnitOrderAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UUnitOrderAbilitySystemComponent* UUnitOrderAttributeSet::GetUnitOrderAbilitySystemComponent() const
{
	return Cast<UUnitOrderAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
