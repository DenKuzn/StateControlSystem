// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyAbilitySystemComponent.h"

#include "Abilities/AbilityData/BoxStrategyAbilityDataAsset.h"
#include "Abilities/Settings/BoxStrategyAbilitySettings.h"
#include "Abilities/BoxStrategyGameplayAbility.h"
#include "BoxStrategy/GeneralDebugMacroses.h"


// Sets default values for this component's properties
UBoxStrategyAbilitySystemComponent::UBoxStrategyAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBoxStrategyAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	{
#if GAME_DEBUG_BUILDS
		if ( UnitTypeList.Num() == 0 )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). UnitTypeList.Num() == 0"), *FString(__FUNCTION__) );
			return;
		}

		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitTypeList[0], );

#endif
	}

	SetNewUnitType( UnitTypeList[ 0 ]->UnitTypeTag, true );
	// ...
}

// Called every frame
void UBoxStrategyAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UBoxStrategyAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded( Handle, Ability, bWasCancelled );

	// Предполагается, что если абилка была отменена, то это потому, что была запущена новая абилка.
	if ( bWasCancelled )
	{
		return;
	}

	// Если абилка завершилась корректно, то значит, пока проверить, есть ли еще приказы.
	ExecuteNextUnitOrder();
}


void UBoxStrategyAbilitySystemComponent::SetNewUnitType(FGameplayTag NewUnitType, bool bStartAutoAbility)
{
	{
#if GAME_DEBUG_BUILDS
		if ( NewUnitType == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). NewUnitType == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	if ( NewUnitType == GetCurrentUnitType() )
	{
		return;
	}

	UBoxStrategyUnitAbilityList* NewUnitTypeData = nullptr;

	for ( auto* UnitType : UnitTypeList )
	{
		if ( UnitType->UnitTypeTag == NewUnitType )
		{
			NewUnitTypeData = UnitType;
			break;
		}
	}

	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewUnitTypeData, );
#endif
	}

	// Remove all abilities
	ClearAllAbilities();

	// Меняем только сейчас, чтобы абилки при закрытии могли не оишибиться с типом, если вдруг это им важно.
	CurrentUnitType = NewUnitTypeData;

	// ADd all Unit Abilities
	for ( auto& UnitAbilityTag : CurrentUnitType->AbilityTagList )
	{
		// Search ability in UnitAbilitiesList.
		AddNewUnitAbility( UnitAbilityTag );
	}

	// Добавить дефолтную абилку.
	AddNewUnitAbility( CurrentUnitType->DefaultAbilityTag );

	// добавить автоматическую абилку.
	AddNewUnitAbility( CurrentUnitType->AutoAbilityTag );

	// Активировать автоматическую абилку.
	if ( bStartAutoAbility )
	{
		ExecuteNextUnitOrder();
	};

	// Сообщить, что произошла смена типа юнита.
	OnUnitTypeChanged.Broadcast( NewUnitType );
}

FGameplayTag UBoxStrategyAbilitySystemComponent::GetCurrentUnitType()
{
	if ( IsValid( CurrentUnitType ) )
	{
		return CurrentUnitType->UnitTypeTag;
	}

	return FGameplayTag::EmptyTag;

	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( CurrentUnitType, FGameplayTag::EmptyTag );
#endif
	}

	return CurrentUnitType->UnitTypeTag;
}

const TArray<UBoxStrategyUnitAbilityList*>& UBoxStrategyAbilitySystemComponent::GetAllUnitTypeList()
{
	return UnitTypeList;
}

bool UBoxStrategyAbilitySystemComponent::GetUnitTypeDescription(FBoxStrategyObjectDescription& OutUnitTypeDescription, FGameplayTag SearchedUnitTypeTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( SearchedUnitTypeTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). SearchedUnitTypeTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return false;
		}
#endif
	}


	if ( !IsValid( CurrentUnitType ) )
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( CurrentUnitType )"), *FString(__FUNCTION__) );
		return false;
	}

	OutUnitTypeDescription = CurrentUnitType->UnitTypeDescription;
	return true;
}

const UBoxStrategyUnitAbilityList* UBoxStrategyAbilitySystemComponent::GetCurrentUnitTypeData()
{
	return CurrentUnitType;
}

bool UBoxStrategyAbilitySystemComponent::GetUnitAbilityData(FBoxStrategyAbilityData& OutAbilityData, FGameplayTag SearchedAbilityTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( SearchedAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). SearchedUnitTypeTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return false;
		}
#endif
	}

	for ( auto& UnitAbilityData : AllUnitAbilitiesDataAsset->AllUnitAbilities )
	{
		if ( UnitAbilityData.AbilityTag == SearchedAbilityTag )
		{
			OutAbilityData = UnitAbilityData;
			return true;
		}
	}

	ensureAlwaysMsgf( false, TEXT("%s(). Ability with tag %s not found"), *FString(__FUNCTION__), *SearchedAbilityTag.ToString() );
	return false;
}

void UBoxStrategyAbilitySystemComponent::AddNewUnitAbility(FGameplayTag AbilityTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( AbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	FBoxStrategyAbilityData UnitAbilityData;
	if ( GetUnitAbilityData( UnitAbilityData, AbilityTag ) )
	{
		UGameplayAbility* AbilityCDO = UnitAbilityData.UnitAbility->GetDefaultObject<UGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityCDO );
		{
#if GAME_DEBUG_BUILDS
			if ( !IsValid( AbilitySpec.Ability ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilitySpec.Ability)"), *FString(__FUNCTION__) );
				return;
			}

			if ( IsAbilityAlreadyLoaded( UnitAbilityData.UnitAbility ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). FindAbilitySpecFromClass( AbilityCDO->GetClass() )"), *FString(__FUNCTION__) );
				return;
			}
#endif
		}

		FGameplayAbilitySpecHandle AbilitySpecHandle = GiveAbility( AbilitySpec );
		FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle( AbilitySpecHandle );
		{
#if GAME_DEBUG_BUILDS
			if ( !Spec )
			{
				ensureAlwaysMsgf( false, TEXT("%s().  !Spec"), *FString(__FUNCTION__) );
				return;
			}
#endif
		}

		UBoxStrategyGameplayAbility* UnitAbility = Cast<UBoxStrategyGameplayAbility>( Spec->GetPrimaryInstance() );
		{
#if GAME_DEBUG_BUILDS
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitAbility, );

			// We need to be sure that is not CDO.
			if ( UnitAbility->HasAnyFlags( RF_ClassDefaultObject ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). UnitAbility->HasAnyFlags( RF_ClassDefaultObject )"), *FString(__FUNCTION__) );
				return;
			}
#endif
		}

		// Добавить абилке тег, чтобы она понимала, какая она.
		UnitAbility->SetAbilityTag( UnitAbilityData.AbilityTag );
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !GetUnitAbilityData( OutAbilityData, AbilityTag )"), *FString(__FUNCTION__) );
	}
}

void UBoxStrategyAbilitySystemComponent::RemoveUnitAbility(FGameplayTag AbilityTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( AbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	FBoxStrategyAbilityData UnitAbilityData;
	if ( GetUnitAbilityData( UnitAbilityData, AbilityTag ) )
	{
		UGameplayAbility* AbilityCDO = UnitAbilityData.UnitAbility->GetDefaultObject<UGameplayAbility>();

		if ( FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass( AbilityCDO->GetClass() ) )
		{
			ClearAbility( Spec->Handle );
		}
		else
		{
			ensureAlwaysMsgf( false, TEXT("%s(). ! FGameplayAbilitySpec* Spec"), *FString(__FUNCTION__) );
			return;
		}
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !GetUnitAbilityData( OutAbilityData, AbilityTag )"), *FString(__FUNCTION__) );
	}
}

void UBoxStrategyAbilitySystemComponent::AddNextUnitAbilityOrderSettings(UBoxStrategyAbilitySettings* NewAbilitySettings, bool bClearOrderList)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewAbilitySettings, );
#endif
	}
	FGameplayTag AbilitSettingsTag = NewAbilitySettings->GetAbilityTag();

	// Check GameplayTag and Ability existing in ActivatableAbilities.
	{
#if GAME_DEBUG_BUILDS
		if ( AbilitSettingsTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilitSettingsTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}

		// Проверить, загружена ли такая абилка с этим тегом в компонент.
		FBoxStrategyAbilityData UnitAbilityData;
		if ( GetUnitAbilityData( UnitAbilityData, AbilitSettingsTag ) )
		{
			if ( !IsAbilityAlreadyLoaded( UnitAbilityData.UnitAbility ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). !(FindAbilitySpecFromClass( AbilityCDO->GetClass() ))"), *FString(__FUNCTION__) );
				return;
			}
		}
		else
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !GetUnitAbilityData( UnitAbilityData, AbilitSettingsTag )"), *FString(__FUNCTION__) );
			return;
		}


#endif
	}

	// Очистить список приказов.
	if ( bClearOrderList )
	{
		OrderSettingsList.Reset( 5 );
	}

	// Добавить новый приказ.
	OrderSettingsList.Add( NewAbilitySettings );
}

const UBoxStrategyAbilitySettings* UBoxStrategyAbilitySystemComponent::GetLastUnitAbilitySettings()
{
	return OrderSettingsList.Last();
}

void UBoxStrategyAbilitySystemComponent::ExecuteNextUnitOrder()
{
	CancelAbilities();

	// If OrderList is empty, then try activate AutoAbility.
	if ( OrderSettingsList.Num() == 0 )
	{
		// AutoAbility dont need to use Ability Settings. AutoAbility will not use it. OrderList will be ignored.
		const UBoxStrategyUnitAbilityList* UnitAbilityList = GetCurrentUnitTypeData();
		{
#if GAME_DEBUG_BUILDS
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitAbilityList, );
#endif
		}

		//If AutoAbilityTag is empty, then do nothing.
		if ( UnitAbilityList->AutoAbilityTag == FGameplayTag::EmptyTag )
		{
			return;
		}

		FBoxStrategyAbilityData UnitAutoAbilityData;
		GetUnitAbilityData( UnitAutoAbilityData, UnitAbilityList->AutoAbilityTag );
		{
#if GAME_DEBUG_BUILDS
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitAutoAbilityData.UnitAbility, );
#endif
		}

		UGameplayAbility* AbilityCDO = UnitAutoAbilityData.UnitAbility->GetDefaultObject<UGameplayAbility>();
		{
#if GAME_DEBUG_BUILDS
			if ( !AbilityCDO )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). !AbilityCDO"), *FString(__FUNCTION__) );
				return;
			}
#endif
		}


		if ( !TryActivateAbilityByClass( AbilityCDO->GetClass() ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !TryActivateAbilityByClass( AbilityCDO->GetClass() )"), *FString(__FUNCTION__) );
		}

		return;
	}

	// Get AbilityTag From Order Settings List.
	UBoxStrategyAbilitySettings* AbilitySettings = OrderSettingsList.Last();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilitySettings, );
#endif
	}
	FGameplayTag UnitAbilityTag = AbilitySettings->GetAbilityTag();
	{
#if GAME_DEBUG_BUILDS
		if ( UnitAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag. Settings name: %s."), *FString(__FUNCTION__), *AbilitySettings->GetName() );
			return;
		}
#endif
	}


	// Данный код не верный, так как он проверяет только доступные абилки в списке абилок.
	// Он не учитывает то, что абилку можно добавить в компонент из предмета.
	{
		FBoxStrategyAbilityData UnitAbilityData;
		if ( GetUnitAbilityData( UnitAbilityData, UnitAbilityTag ) )
		{
			{
#if GAME_DEBUG_BUILDS
				GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitAbilityData.UnitAbility, );
#endif
			}

			TryActivateAbilityByClass( UnitAbilityData.UnitAbility );
			// В потоке может быть добавлена новая задача.
		}
		else
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !GetUnitAbilityData( UnitAbilityData, UnitAbilityTag )"), *FString(__FUNCTION__) );
			return;
		}
	}


	OrderSettingsList.Remove( AbilitySettings );
}

void UBoxStrategyAbilitySystemComponent::ExecuteDefaultUnitOrder(UBoxStrategyAbilitySettings* AbilitySettings)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilitySettings, );
#endif
	}

	auto* UnitData = GetCurrentUnitTypeData();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitData, );

		if ( UnitData->DefaultAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag. Settings name: %s."), *FString(__FUNCTION__), *AbilitySettings->GetName() );
			return;
		}
#endif
	}

	ExecuteNewUnitOrder( AbilitySettings, UnitData->DefaultAbilityTag, true );
}

void UBoxStrategyAbilitySystemComponent::ExecuteNewUnitOrder(UBoxStrategyAbilitySettings* AbilitySettings, FGameplayTag UnitAbilityTag, bool bClearOrderList)
{
	{
#if GAME_DEBUG_BUILDS

		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilitySettings, );

		if ( UnitAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag. Settings name: %s."), *FString(__FUNCTION__), *AbilitySettings->GetName() );
			return;
		}
#endif
	}

	AbilitySettings->SetAbilityTag( UnitAbilityTag );
	AddNextUnitAbilityOrderSettings( AbilitySettings, bClearOrderList );
	ExecuteNextUnitOrder();
}

void UBoxStrategyAbilitySystemComponent::GetAllAvailableAbilitiesTags(TArray<FGameplayTag>& OutAbilityTags)
{
	OutAbilityTags.Reset( ActivatableAbilities.Items.Num() + 1 );
	for ( auto& AbilitySpec : ActivatableAbilities.Items )
	{
		UBoxStrategyGameplayAbility* UnitAbility = Cast<UBoxStrategyGameplayAbility>( AbilitySpec.GetPrimaryInstance() );
		{
#if GAME_DEBUG_BUILDS
			if ( !IsValid( UnitAbility ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(UnitAbility)"), *FString(__FUNCTION__) );
				continue;
			}
#endif
		}

		OutAbilityTags.AddUnique( UnitAbility->GetAbilityTag() );
	}

	return;
}

UBoxStrategyAbilityDataAsset* UBoxStrategyAbilitySystemComponent::GetUnitAbilitiesDataAsset()
{
	return AllUnitAbilitiesDataAsset;
}

bool UBoxStrategyAbilitySystemComponent::IsAbilityAlreadyLoaded(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if ( AbilityClass.Get() )
	{
		UGameplayAbility* AbilityCDO = AbilityClass->GetDefaultObject<UGameplayAbility>();
		return FindAbilitySpecFromClass( AbilityCDO->GetClass() ) ? true : false;
	}
	else
	{
		ensureAlwaysMsgf( false, TEXT("%s(). AbilityClass.Get()"), *FString(__FUNCTION__) );
		return false;
	}
}
