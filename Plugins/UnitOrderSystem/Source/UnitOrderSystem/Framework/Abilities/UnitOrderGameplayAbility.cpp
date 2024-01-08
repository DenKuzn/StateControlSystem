// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderGameplayAbility.h"

#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

UUnitOrderGameplayAbility::UUnitOrderGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UUnitOrderGameplayAbility::SetAbilityTag(FGameplayTag NewAbilityTag)
{
	AbilityTags.Reset(1);
	AbilityTags.AddTag(NewAbilityTag);
	//AbilityTag = NewAbilityTag;
}

FGameplayTag UUnitOrderGameplayAbility::GetAbilityTag() const
{

	return AbilityTags.Last();
}

UUnitOrderAbilitySystemComponent* UUnitOrderGameplayAbility::GetUnitOrderAbilitySystemComponent()
{
	return Cast<UUnitOrderAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

const UUnitOrderAbilitySettings* UUnitOrderGameplayAbility::GetMyAbilitySettings()
{
	UUnitOrderAbilitySystemComponent* AbilityComponent = GetUnitOrderAbilitySystemComponent();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(AbilityComponent, nullptr  );
#endif
	}

	return AbilityComponent->GetLastUnitAbilitySettings();
}
