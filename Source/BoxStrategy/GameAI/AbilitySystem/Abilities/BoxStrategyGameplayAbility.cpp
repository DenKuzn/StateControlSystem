// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyGameplayAbility.h"

#include "BoxStrategy/GameAI/AbilitySystem/BoxStrategyAbilitySystemComponent.h"
#include "BoxStrategy/GeneralDebugMacroses.h"

UBoxStrategyGameplayAbility::UBoxStrategyGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBoxStrategyGameplayAbility::SetAbilityTag(FGameplayTag NewAbilityTag)
{
	AbilityTags.Reset(1);
	AbilityTags.AddTag(NewAbilityTag);
	//AbilityTag = NewAbilityTag;
}

FGameplayTag UBoxStrategyGameplayAbility::GetAbilityTag() const
{

	return AbilityTags.Last();
}

UBoxStrategyAbilitySystemComponent* UBoxStrategyGameplayAbility::GetBoxStrategyAbilitySystemComponent()
{
	return Cast<UBoxStrategyAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

const UBoxStrategyAbilitySettings* UBoxStrategyGameplayAbility::GetMyAbilitySettings()
{
	UBoxStrategyAbilitySystemComponent* AbilityComponent = GetBoxStrategyAbilitySystemComponent();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(AbilityComponent, nullptr  );
#endif
	}

	return AbilityComponent->GetLastUnitAbilitySettings();
}
