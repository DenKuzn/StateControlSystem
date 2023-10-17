// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyAbilitySettings.h"

void UBoxStrategyAbilitySettings::SetAbilityTag(FGameplayTag NewTag)
{
	AbilityTag = NewTag;
}

FGameplayTag UBoxStrategyAbilitySettings::GetAbilityTag()
{
		return AbilityTag;
}
