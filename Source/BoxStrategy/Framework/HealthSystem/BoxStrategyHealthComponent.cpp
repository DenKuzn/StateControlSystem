// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyHealthComponent.h"
#include "BoxStrategy/GeneralDebugMacroses.h"


// Sets default values for this component's properties
UBoxStrategyHealthComponent::UBoxStrategyHealthComponent(const FObjectInitializer& ObjectInitializer):
Super( ObjectInitializer )
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBoxStrategyHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBoxStrategyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UBoxStrategyHealthComponent::AddDamage(float NewDamage, FGameplayTag DamageType)
{
	{
#if GAME_DEBUG_BUILDS
		if ( NewDamage <= 0.f )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). NewDamage <= 0.f"), *FString(__FUNCTION__) );
			return;
		}

		if ( DamageType == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). DamageTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	if(IsDeadOrDying())
	{
		return;
	}


	float* DamageMultiplierValue = DamageMultiplier.Find( DamageType );
	if ( DamageMultiplierValue )
	{
		NewDamage *= *DamageMultiplierValue;
	}

	CurrentDamage += NewDamage;

	OnDamageDelegate.Broadcast( NewDamage, DamageType );

	if((BaseHealth - CurrentDamage) <= 0)
	{
		StartDeath();
		return;
	}
}


float UBoxStrategyHealthComponent::GetCurrentHealth() const
{
	return BaseHealth - CurrentDamage;
}

float UBoxStrategyHealthComponent::GetMaxHealth() const
{
	return BaseHealth;
}

float UBoxStrategyHealthComponent::GetCurrentHealthNormalized() const
{
	return GetCurrentHealth() / GetMaxHealth();
}

EBoxStrategyDeathState UBoxStrategyHealthComponent::GetDeathState() const
{
	return DeathState;
}

bool UBoxStrategyHealthComponent::IsDeadOrDying() const
{
	return (DeathState > EBoxStrategyDeathState::NotDead);
}

void UBoxStrategyHealthComponent::StartDeath()
{
	if(IsDeadOrDying())
	{
		return;
	}

	DeathState = EBoxStrategyDeathState::DeathStarted;
	OnDeadStartedDelegate.Broadcast();
}

void UBoxStrategyHealthComponent::FinishDeath()
{
	DeathState = EBoxStrategyDeathState::DeathFinished;
	OnDeadFinishedDelegate.Broadcast();
}

void UBoxStrategyHealthComponent::DamageSelfDestruct(bool bFellOutOfWorld)
{
	if(IsDeadOrDying())
	{
		return;
	}

	DeathState = EBoxStrategyDeathState::DeathStarted;
	OnDeadStartedDelegate.Broadcast();
}
