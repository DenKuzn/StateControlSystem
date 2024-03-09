// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOrderAbilitySystemComponent.h"

#include "Engine/AssetManager.h"

#include "UnitOrderSystem/Framework/UnitData/UnitOrderUnitTypeData.h"
#include "UnitOrderSystem/Framework/Abilities/Settings/UnitOrderAbilitySettings.h"
#include "UnitOrderSystem/Framework/Abilities/UnitOrderGameplayAbility.h"
#include "UnitOrderSystem/Framework/UnitOrderSubsystem.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

DEFINE_LOG_CATEGORY_STATIC( LogUnitOrderSystemAbilityComponent, Log, All );

// Sets default values for this component's properties
UUnitOrderAbilitySystemComponent::UUnitOrderAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UUnitOrderAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// 1. Component knows his UnitType and deafult UnitSubtype.
	// We need to check all data in that (it must be valid).
	if ( !ValidateSettings() )
	{
		// We cannot work without knoladge whata we are and who we are.
		UE_LOG( LogUnitOrderSystemAbilityComponent, Error, TEXT("%s(). !ValidateSettings(). In Class: %s"), *FString(__FUNCTION__), *GetOwner()->GetName() );
		return;
	}

	CurrentOrderControllerTypeTag = DefaultOrderControllerTypeTag;

	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, );
#endif
	}

	if ( !UnitOrderSubsystem->IsAllUnitDataInitialized() )
	{
		UE_LOG(LogUnitOrderSystemAbilityComponent, Warning, TEXT("%s(). !UnitOrderSubsystem->IsAllUnitDataInitialized()"), *FString(__FUNCTION__) );
		UnitOrderSubsystem->LoadingUnitDataComplited.AddDynamic( this, &UUnitOrderAbilitySystemComponent::InitializeUnitAbilityComponent );
		return;
	}

	InitializeUnitAbilityComponent();
}

// Called every frame
void UUnitOrderAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UUnitOrderAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded( Handle, Ability, bWasCancelled );

	// If the ability was canceled that means new ability started. When an ability is finished by self it always FinishAbility() without cancelation.

	// If it was new controller then it assigned his tag to the component
	// ( When this ability will finish the work ) && ( no one order was added ) then
	// ( Current Order Type Tag will be checked ) If it is not default then ASC will send message to all controllers that have control over it.
	// ( If it is default then try execute Next Order and run AutoAbility by default. )
	if ( bWasCancelled )
	{
		return;
	}

	if(CurrentOrderControllerTypeTag != DefaultOrderControllerTypeTag)
	{
		OnAbilityEndedWithComponentNonDefaultState.Broadcast( CurrentOrderControllerTypeTag );
		return;
	}

	// Если абилка завершилась корректно, то значит, пока проверить, есть ли еще приказы.
	ExecuteNextUnitOrder();
}


#pragma region UNIT TYPE SETTINGS

UUnitOrderUnitTypeData* UUnitOrderAbilitySystemComponent::GetUnitTypeData()
{
	return BaseUnitType;
}

FGameplayTag UUnitOrderAbilitySystemComponent::GetUnitTypeTag()
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( BaseUnitType, FGameplayTag::EmptyTag );
#endif
	}

	return BaseUnitType->UnitTypeTag;
}

bool UUnitOrderAbilitySystemComponent::GetUnitTypeDescription(FObjectDescription& OutUnitTypeDescription)
{
	//Check if UnitType is valid.
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( BaseUnitType, false );
#endif
	}

	OutUnitTypeDescription = BaseUnitType->UnitTypeDescription;
	return true;
}

bool UUnitOrderAbilitySystemComponent::GetUnitAbilityData(FUnitOrderAbilityData& OutAbilityData, const FGameplayTag& SearchedAbilityTag)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( BaseUnitType, false );

		if ( SearchedAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). SearchedUnitTypeTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return false;
		}

#endif
	}

	// Получить Subsystem.
	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, false );
#endif
	} // Check if Subsystem is valid.

	return UnitOrderSubsystem->GetUnitAbilityData( BaseUnitType->UnitTypeTag, SearchedAbilityTag, OutAbilityData );
}

#pragma endregion UNIT TYPE SETTINGS


#pragma region UNIT SUBTYPE SETTINGS

void UUnitOrderAbilitySystemComponent::SetNewUnitSubtype(const FGameplayTag& NewUnitSubtype, bool bStartAutoAbility)
{
	{
#if GAME_DEBUG_BUILDS
		if ( NewUnitSubtype == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). NewUnitType == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
			return;
		}

		if ( !NewUnitSubtype.MatchesTag( GetUnitTypeTag() ) )
		{
			ensureAlwaysMsgf( false,
			                  TEXT("%s().!NewUnitSubtype.MatchesTag( GetUnitTypeTag() ). SubtypeTag: %s. UnitTypeTag:%s "),
			                  *FString(__FUNCTION__),
			                  *NewUnitSubtype.ToString(),
			                  *GetUnitTypeTag().ToString() );
			return;
		}
#endif
	}

	if ( NewUnitSubtype == GetCurrentUnitSubtypeTag() )
	{
		UE_LOG( LogUnitOrderSystemAbilityComponent, Warning, TEXT("%s(). NewUnitSubtype == GetCurrentUnitSubtypeTag()"), *FString(__FUNCTION__) );
		return;
	}

	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, );
#endif
	} // Check if Subsystem is valid.

	UUnitOrderUnitSubtypeData* NewUnitSubtypeData = UnitOrderSubsystem->GetUnitSubtypeData( NewUnitSubtype );
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewUnitSubtypeData, );
#endif
	}

	// Меняем только сейчас, чтобы абилки при закрытии могли не оишибиться с типом, если вдруг это им важно.
	CurrentUnitSubtype = NewUnitSubtypeData;

	CancelAbilities();

	// Активировать автоматическую абилку.
	if ( bStartAutoAbility )
	{
		ExecuteNextUnitOrder();
	};

	// Сообщить, что произошла смена типа юнита.
	OnUnitTypeChanged.Broadcast( NewUnitSubtype );
}

const FGameplayTag& UUnitOrderAbilitySystemComponent::GetCurrentUnitSubtypeTag() const
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( CurrentUnitSubtype, FGameplayTag::EmptyTag );
#endif
	}

	return CurrentUnitSubtype->UnitSubtypeTag;
}

const UUnitOrderUnitSubtypeData* UUnitOrderAbilitySystemComponent::GetCurrentUnitSubtypeData()
{
	return CurrentUnitSubtype;
}

bool UUnitOrderAbilitySystemComponent::GetCurrentUnitSubtypeDescription(FObjectDescription& OutUnitTypeDescription)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( CurrentUnitSubtype, false );
#endif
	}

	OutUnitTypeDescription = CurrentUnitSubtype->UnitSubtypeDescription;
	return true;
}

void UUnitOrderAbilitySystemComponent::GetCurrentUnitSubtypeAbilityTags(TArray<FGameplayTag>& OutAbilityTags)
{
	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, );
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( CurrentUnitSubtype, );
#endif
	} // Check if Subsystem is valid.

	UnitOrderSubsystem->GetUnitSubtypeAbilityTags( CurrentUnitSubtype->UnitSubtypeTag, OutAbilityTags );
}

#pragma endregion UNIT SUBTYPE SETTINGS


#pragma region ORDER SETTINGS


void UUnitOrderAbilitySystemComponent::SetNewOrderType(const FGameplayTag& NewOrderType)
{
	CurrentOrderControllerTypeTag = NewOrderType;
}

const FGameplayTag& UUnitOrderAbilitySystemComponent::GetCurrentOrderTypeTag() const
{
	return CurrentOrderControllerTypeTag;
}

void UUnitOrderAbilitySystemComponent::AddNextUnitAbilityOrderSettings(UUnitOrderAbilitySettings* NewAbilitySettings, bool bClearOrderList)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewAbilitySettings, );

		if ( NewAbilitySettings->GetAbilityTag() == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilitSettingsTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
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

const UUnitOrderAbilitySettings* UUnitOrderAbilitySystemComponent::GetLastUnitAbilitySettings() const
{
	return OrderSettingsList.Last();
}

void UUnitOrderAbilitySystemComponent::ExecuteNextUnitOrder()
{
	CancelAbilities();

	// If OrderList is empty, then try activate AutoAbility.
	if ( OrderSettingsList.Num() == 0 )
	{
		// AutoAbility dont need to use Ability Settings. AutoAbility will not use it. OrderList will be ignored.
		const UUnitOrderUnitSubtypeData* UnitAbilityList = GetCurrentUnitSubtypeData();
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

		ExecuteNewUnitOrder( NewObject<UUnitOrderAbilitySettings>(), UnitAbilityList->AutoAbilityTag, true );

		return;
	} // if flow down to this part function will return in any case.


	// Get AbilityTag From Order Settings List.
	UUnitOrderAbilitySettings* AbilitySettings = OrderSettingsList.Last();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilitySettings, );
#endif
	}

	FGameplayTag UnitActivatableAbilityTag = AbilitySettings->GetAbilityTag();
	{
#if GAME_DEBUG_BUILDS
		if ( UnitActivatableAbilityTag == FGameplayTag::EmptyTag )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). AbilityTag == FGameplayTag::EmptyTag. Settings name: %s."), *FString(__FUNCTION__), *AbilitySettings->GetName() );
			return;
		}
#endif
	}

	// Перебрать все существующие добавленные абилки и найти в них абилку с автозапускным тегом.
	UUnitOrderGameplayAbility* UnitAbility = FindAbilityInstanceByTag( UnitActivatableAbilityTag );
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitAbility, );
#endif
	}

	// Данный код не верный, так как он проверяет только доступные абилки в списке абилок.
	// Он не учитывает то, что абилку можно добавить в компонент из предмета.
	TryActivateAbilityByClass( UnitAbility->GetClass() );

	// В потоке может быть добавлена новая задача, поэтому удалять по ссылке, а не по индексу.
	OrderSettingsList.Remove( AbilitySettings );
}

void UUnitOrderAbilitySystemComponent::ExecuteDefaultUnitOrder(UUnitOrderAbilitySettings* AbilitySettings)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilitySettings, );
#endif
	}

	auto* UnitData = GetCurrentUnitSubtypeData();
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

void UUnitOrderAbilitySystemComponent::ExecuteNewUnitOrder(UUnitOrderAbilitySettings* AbilitySettings, const FGameplayTag& UnitAbilityTag, bool bClearOrderList)
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

#pragma endregion ORDER SETTINGS


UUnitOrderGameplayAbility* UUnitOrderAbilitySystemComponent::FindAbilityInstanceByTag(const FGameplayTag& AbilityTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( ActivatableAbilities.Items.Num() == 0 )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). ActivatableAbilities.Items.Num() == 0"), *FString(__FUNCTION__) );
			return nullptr;
		}
#endif
	}

	for ( auto& AbilitySpec : ActivatableAbilities.Items )
	{
		UUnitOrderGameplayAbility* UnitAbility = Cast<UUnitOrderGameplayAbility>( AbilitySpec.GetPrimaryInstance() );
		{
#if GAME_DEBUG_BUILDS
			if ( !IsValid( UnitAbility ) )
			{
				ensureAlwaysMsgf( false, TEXT("%S(). !IsValid( UnitAbility )"), *FString(__FUNCTION__) );
				continue;
			}
#endif
		}

		if ( UnitAbility->GetAbilityTag() == AbilityTag )
		{
			return UnitAbility;
		}
	}

	UE_LOG( LogUnitOrderSystemAbilityComponent, Warning, TEXT("%s(). Ability with tag: %s not found."), *FString(__FUNCTION__), *AbilityTag.ToString() );
	return nullptr;
}

bool UUnitOrderAbilitySystemComponent::IsAbilityAlreadyLoaded(TSubclassOf<UGameplayAbility> AbilityClass)
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

void UUnitOrderAbilitySystemComponent::UnitSubtypeDataAssetesLoaded()
{
}

bool UUnitOrderAbilitySystemComponent::ValidateSettings()
{
#if GAME_DEBUG_BUILDS
	bool bAllGood = true;

	if ( !IsValid( BaseUnitType ) )
	{
		bAllGood = false;
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(BaseUnitType). In Class: %s."), *FString(__FUNCTION__), *GetOwner()->GetName() );
	}
	else if ( BaseUnitType->UnitTypeTag == FGameplayTag::EmptyTag )
	{
		bAllGood = false;
		ensureAlwaysMsgf( false,
		                  TEXT("%s(). BaseUnitType->UnitTypeTag == FGameplayTag::EmptyTag. In Class: %s."),
		                  *FString(__FUNCTION__),
		                  *GetOwner()->GetName() );
	}

	if ( !IsValid( CurrentUnitSubtype ) )
	{
		bAllGood = false;
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(CurrentUnitSubtype). In Class: %s."), *FString(__FUNCTION__), *GetOwner()->GetName() );
	}
	else if ( CurrentUnitSubtype->UnitSubtypeTag == FGameplayTag::EmptyTag )
	{
		bAllGood = false;
		ensureAlwaysMsgf( false,
		                  TEXT("%s(). CurrentUnitSubtype->UnitSubtypeTag == FGameplayTag::EmptyTag. In Class: %s."),
		                  *FString(__FUNCTION__),
		                  *GetOwner()->GetName() );
	}

	if(DefaultOrderControllerTypeTag == FGameplayTag::EmptyTag)
	{
		bAllGood = false;
		ensureAlwaysMsgf( false,
		                  TEXT("%s(). DefaultOrderTypeTag == FGameplayTag::EmptyTag. In Class: %s."),
		                  *FString(__FUNCTION__),
		                  *GetOwner()->GetName() );
	}

	return bAllGood;

#endif
	return true;
}

UUnitOrderSubsystem* UUnitOrderAbilitySystemComponent::GetUnitOrderSubsystem()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUnitOrderSubsystem>();
}

void UUnitOrderAbilitySystemComponent::AddNewUnitAbility(const FUnitOrderAbilityData& NewUnitAbilityData)
{
	UClass* AbilityClass = NewUnitAbilityData.UnitAbilityClass.LoadSynchronous();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( AbilityClass, );
#endif
	}

	UGameplayAbility* AbilityCDO = AbilityClass->GetDefaultObject<UGameplayAbility>();
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec( AbilityCDO );
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( AbilitySpec.Ability ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilitySpec.Ability)"), *FString(__FUNCTION__) );
			return;
		}

		if ( IsAbilityAlreadyLoaded( AbilityClass ) )
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

	UUnitOrderGameplayAbility* UnitAbility = Cast<UUnitOrderGameplayAbility>( Spec->GetPrimaryInstance() );
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
	UnitAbility->SetAbilityTag( NewUnitAbilityData.AbilityTag );
}

void UUnitOrderAbilitySystemComponent::InitializeUnitAbilityComponent()
{
	FUNCTION_START_LOG();
	UUnitOrderSubsystem* UnitOrderSubsystem = GetUnitOrderSubsystem();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( BaseUnitType, );
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderSubsystem, );

		if ( !UnitOrderSubsystem->IsAllUnitDataInitialized() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !UnitOrderSubsystem->IsAllUnitDataInitialized()"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	UnitOrderSubsystem->LoadingUnitDataComplited.RemoveDynamic( this, &UUnitOrderAbilitySystemComponent::InitializeUnitAbilityComponent );
	const TArray<FUnitOrderAbilityData>* UnitAbilities = UnitOrderSubsystem->GetAllUnitTypeAbilityData( BaseUnitType->UnitTypeTag );
	{
#if GAME_DEBUG_BUILDS
		if ( !UnitAbilities )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !UnitAbilities"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	if ( UnitAbilities->Num() == 0 )
	{
		UE_LOG( LogUnitOrderSystemAbilityComponent, Warning, TEXT("%s(). UnitAbilities->Num() == 0"), *FString(__FUNCTION__) );
		return;
	}

	for ( auto& UnitAbility : *UnitAbilities )
	{
		AddNewUnitAbility( UnitAbility );
	}

	ExecuteNextUnitOrder();
	FUNCTION_END_LOG();
}


/*
void UUnitOrderAbilitySystemComponent::RemoveUnitAbility(FGameplayTag AbilityTag)
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

	FUnitOrderAbilityData UnitAbilityData;
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

*/
