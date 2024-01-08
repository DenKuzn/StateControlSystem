// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderAbilitySettings.h"

void UUnitOrderAbilitySettings::SetAbilityTag(FGameplayTag NewTag)
{
	AbilityTag = NewTag;
}

FGameplayTag UUnitOrderAbilitySettings::GetAbilityTag()
{
		return AbilityTag;
}
