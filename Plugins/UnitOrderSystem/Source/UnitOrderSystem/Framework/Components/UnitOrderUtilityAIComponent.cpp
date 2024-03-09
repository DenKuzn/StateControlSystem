// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderUtilityAIComponent.h"

#include "UnitOrderAbilitySystemComponent.h"
#include "UnitOrderSystem/Framework/UnitOrderSubsystem.h"
#include "UnitOrderSystem/Framework/UnitData/UnitOrderAbilityData.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "UnitOrderSystem/Framework/Abilities/Settings/UnitOrderAbilitySettings.h"

DEFINE_LOG_CATEGORY_STATIC( LogUnitOrderUtilityAIComponent, Log, All );


// Sets default values for this component's properties
UUnitOrderUtilityAIComponent::UUnitOrderUtilityAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUnitOrderUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

UUnitOrderAbilitySystemComponent* UUnitOrderUtilityAIComponent::GetUnitOrderAbilitySystemComponent()
{
	return GetOwner()->GetComponentByClass<UUnitOrderAbilitySystemComponent>();
}

// When Work Schedule will work properly, this function will be called from it. And need to update this function for Work Schedule as well.
// If schdelude will not change current behavior, this function must take it into account and do nothing.
void UUnitOrderUtilityAIComponent::ActivateNewBehavior()
{
	UUnitOrderAbilitySystemComponent* UnitOrderAbilitySystemComponent = GetUnitOrderAbilitySystemComponent();
	{
		if(!IsValid( UnitOrderAbilitySystemComponent ))
		{
			UE_LOG(LogUnitOrderUtilityAIComponent, Warning, TEXT("%s(). UnitOrderAbilitySystemComponent is not valid. I cannot work without UO ASC"), *FString(__FUNCTION__) );
			return;
		}
	}

	
	// Claim that ASC now is working with this component.
	UnitOrderAbilitySystemComponent->OnAbilityEndedWithComponentNonDefaultState.AddUniqueDynamic( this, &UUnitOrderUtilityAIComponent::AbilityWasEnded );
	if(UnitOrderAbilitySystemComponent->GetCurrentOrderTypeTag() != UnitOrderTypeTag)
	{
		UnitOrderAbilitySystemComponent->SetNewOrderType( UnitOrderTypeTag );
	}


	FindAndExecuteNewAbility();
}







UUnitOrderSubsystem* UUnitOrderUtilityAIComponent::GetUnitOrderSubsystem()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUnitOrderSubsystem>();
}

FGameplayTag UUnitOrderUtilityAIComponent::GetNextAbilityTag(const FGameplayTag& UnitSubtype)
{
	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, FGameplayTag{} );
#endif
	}

	const TArray<FUnitOrderUtilityAIAbilityData_Struct>* AvailableUtilityAIAbilities = UnitOrderSubsystem->GetAvailableUtilityAIAbilities( UnitSubtype );
	{
		if ( !AvailableUtilityAIAbilities )
		{
			UE_LOG( LogUnitOrderUtilityAIComponent, Warning, TEXT("%s(). AvailableUtilityAIAbilities is not valid."), *FString(__FUNCTION__) );
			return FGameplayTag{};
		}

		if ( AvailableUtilityAIAbilities->Num() == 0 )
		{
			UE_LOG( LogUnitOrderUtilityAIComponent, Warning, TEXT("%s(). AvailableUtilityAIAbilities->Num() == 0."), *FString(__FUNCTION__) );
			return FGameplayTag{};
		}
	}

	FUnitOrderUtilityAIAbilityData_Struct TheBestAbilityData;
	float TheBestAbilityWeight = -1.f;

	for ( const FUnitOrderUtilityAIAbilityData_Struct& NewUtilityAIAbilityData : *AvailableUtilityAIAbilities )
	{
		const UEvaluationAbilityClass* EvaluationClassCDO = NewUtilityAIAbilityData.EvaluationAbilityClass.GetDefaultObject();
		{
			if ( !IsValid( EvaluationClassCDO ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). EvaluationClassCDO is not valid."), *FString(__FUNCTION__) );
				continue;
			}
		}

		float NewAbilityWeight = EvaluationClassCDO->GetAbilityWeight( GetOwner() );
		{
			if ( TheBestAbilityWeight < NewAbilityWeight )
			{
				TheBestAbilityWeight = NewAbilityWeight;
				TheBestAbilityData = NewUtilityAIAbilityData;
				continue;
			}

			// Если 2 абилки собрали одинаковый вес, то сравнение идет на приоритет.
			// Если приоритет одинаковый, то выбор падает рандомно.
			if ( TheBestAbilityData.AbilityTag.IsValid() && NewAbilityWeight == TheBestAbilityWeight )
			{
				if ( TheBestAbilityData.AbilityPriority < NewUtilityAIAbilityData.AbilityPriority )
				{
					TheBestAbilityData = NewUtilityAIAbilityData;
				}
				else if ( TheBestAbilityData.AbilityPriority == NewUtilityAIAbilityData.AbilityPriority )
				{
					if ( FMath::RandBool() )
					{
						TheBestAbilityData = NewUtilityAIAbilityData;
					}
				}
				continue;
			}
		}
	}

	return TheBestAbilityData.AbilityTag;
}

void UUnitOrderUtilityAIComponent::AbilityWasEnded(FGameplayTag ControlTypeTag)
{
	if(UnitOrderTypeTag == ControlTypeTag)
	{
		FindAndExecuteNewAbility();
	}
	else
	{
		UUnitOrderAbilitySystemComponent* UnitOrderAbilitySystemComponent = GetUnitOrderAbilitySystemComponent();
		{
			{
#if GAME_DEBUG_BUILDS
				GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderAbilitySystemComponent, );
#endif
			}
		}

		UnitOrderAbilitySystemComponent->OnAbilityEndedWithComponentNonDefaultState.RemoveDynamic( this, &UUnitOrderUtilityAIComponent::AbilityWasEnded );
	}
}

void UUnitOrderUtilityAIComponent::FindAndExecuteNewAbility()
{
	UUnitOrderAbilitySystemComponent* UnitOrderAbilitySystemComponent = GetUnitOrderAbilitySystemComponent();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderAbilitySystemComponent, );
#endif
	}

	FGameplayTag UtilityAIAbilityTag = GetNextAbilityTag(UnitOrderAbilitySystemComponent->GetCurrentUnitSubtypeTag());
	{
#if GAME_DEBUG_BUILDS
		if(!UtilityAIAbilityTag.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !TheBestAbilityData.AbilityTag.IsValid()"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	UnitOrderAbilitySystemComponent->ExecuteNewUnitOrder( NewObject<UUnitOrderAbilitySettings>(), UtilityAIAbilityTag );
}
