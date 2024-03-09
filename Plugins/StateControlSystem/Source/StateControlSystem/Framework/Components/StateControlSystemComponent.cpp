// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "StateControlSystemComponent.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "StateControlSystem/Framework/StateControlDataAsset.h"
#include "StateControlSystem/Framework/StateControlWidget.h"


// Sets default values for this component's properties
UStateControlSystemComponent::UStateControlSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStateControlSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// If it is not controlled by Player, then end function.
	/*
	{
		if ( APawn* Player = Cast<APawn>( GetOwner() ) )
		{
			if ( !Player->IsLocallyControlled() )
			{
				return;
			}
		}
		else
		{
			if ( GetWorld() && GetWorld()->GetFirstPlayerController() )
			{
				if ( !GetWorld()->GetFirstPlayerController()->IsLocalController() )
				{
					return;
				}
			}
			else
			{
				ensureAlwaysMsgf( false, TEXT("%s(). GetWorld() && GetWorld()->GetFirstPlayerController() invalid"), *FString(__FUNCTION__) );
			}
		}
	}
	*/ // Это одиночная игра, поэтому нам не требуется проверка. Более того, SCSC Висит на PlayerState.

	//check BaseStateControl.
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( StateControlData, );
		if ( !IsValid( StateControlData->DefaultStateControlData.StateControlClass ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( StateControlData->DefaultStateControlData.StateControlClass )"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	UStateControl* NewBaseStateControl = NewObject<UStateControl>( this, StateControlData->DefaultStateControlData.StateControlClass );
	NewBaseStateControl->InitializeStateControl( StateControlData->DefaultStateControlData.StateWidgetClass );

	StateControlsStorage.Add( StateControlData->DefaultStateControlData.StateControlTag, NewBaseStateControl );
	CurrentActiveStates.Add( NewBaseStateControl );

	for ( FStateControlData& StateControlElem : StateControlData->AllStateControlData )
	{
		//check data.
		{
#if GAME_DEBUG_BUILDS
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( StateControlElem.StateControlClass.Get(), );
			if ( StateControlElem.StateControlTag == FGameplayTag::EmptyTag )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). StateControlData.StateControlTag == FGameplayTag::EmptyTag"), *FString(__FUNCTION__) );
				return;
			}
#endif
		}

		UStateControl* NewStateControl = NewObject<UStateControl>( this, StateControlElem.StateControlClass );
		NewStateControl->InitializeStateControl( StateControlElem.StateWidgetClass );
		StateControlsStorage.Add( StateControlElem.StateControlTag, NewStateControl );
	}
	// ...

	NewBaseStateControl->ActivateState();
}


// Called every frame
void UStateControlSystemComponent::TickComponent(float DeltaTime,
                                                 ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

UStateControl* UStateControlSystemComponent::SetNewActiveStateByTag(FGameplayTag NewStateTag, bool NeedAutoActivate)
{
	{
#if GAME_DEBUG_BUILDS
		if ( !NewStateTag.IsValid() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !NewStateTag.IsValid()"), *FString(__FUNCTION__) );
			return nullptr;
		}
#endif
	}

	UStateControl** StateControlInStorage = StateControlsStorage.Find( NewStateTag );
	{
#if GAME_DEBUG_BUILDS
		if ( !StateControlInStorage || !IsValid( *StateControlInStorage ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). StateControl Invalid by tag: %s."), *FString(__FUNCTION__), *NewStateTag.GetTagName().ToString() );
			return nullptr;
		}

		if ( CurrentActiveStates.Contains( *StateControlInStorage ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). StateControl already active by tag: %s."), *FString(__FUNCTION__), *NewStateTag.GetTagName().ToString() );
			return nullptr;
		}
#endif
	}

	UStateControl* LastStateControl = CurrentActiveStates.Last();

	// Блокируем горячие клавиши у предыдущего StateControl.
	if ( IsValid( LastStateControl ) )
	{
		LastStateControl->SleepState();
	}

	CurrentActiveStates.Add( ( *StateControlInStorage ) );

	if ( NeedAutoActivate )
	{
		( *StateControlInStorage )->ActivateState();
	}

	OnStateWasChanged.Broadcast();
	return ( *StateControlInStorage );
}

void UStateControlSystemComponent::DeactivateState(UStateControl* SearchedStateControl)
{
	//Checker.
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( SearchedStateControl, )
#endif
	}
	CurrentActiveStates.Remove( SearchedStateControl );
	SearchedStateControl->DeactivateState();

	UStateControl* LastStateControl = CurrentActiveStates.Last();

	if ( IsValid( LastStateControl ) && LastStateControl->GetStateControlStatus() == EStateControlStatus::ME_Sleeping )
	{
		LastStateControl->WakeUpState();
	}

	OnStateWasChanged.Broadcast();
}

UStateControl* UStateControlSystemComponent::DeactivateStateByTag(FGameplayTag SearchedStateTag)
{
	{
#if GAME_DEBUG_BUILDS
		if ( !SearchedStateTag.IsValid() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !NewStateTag.IsValid()"), *FString(__FUNCTION__) );
			return nullptr;
		}
#endif
	}

	UStateControl** StageInStorage = StateControlsStorage.Find( SearchedStateTag );
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( *StageInStorage ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). StateControl Invalid by tag: %s."), *FString(__FUNCTION__), *SearchedStateTag.GetTagName().ToString() );
			return nullptr;
		}
#endif
	}

	DeactivateState( ( *StageInStorage ) );

	OnStateWasChanged.Broadcast();
	return ( *StageInStorage );
}

bool UStateControlSystemComponent::IsTagForStateControl(FGameplayTag SearchedStateTag, UStateControl* SearchedStateControl)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( SearchedStateControl, false );
		if ( !SearchedStateTag.IsValid() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !SearchedStateTag.IsValid()"), *FString(__FUNCTION__) );
			return false;
		}
#endif
	}

	return ( *StateControlsStorage.Find( SearchedStateTag ) ) == SearchedStateControl;
}

bool UStateControlSystemComponent::IsStateControlActive(UStateControl* StateControl)
{
	for ( auto* StateControlElement : CurrentActiveStates )
	{
		if ( StateControlElement == StateControl )
		{
			return true;
		}
	}

	return false;
}

bool UStateControlSystemComponent::IslocalyControlled()
{

	return true;

	//TODO: Компонент висит на PlayerState и работает для одиночного режима, поэтому всегда true.


	APawn* Pawn = Cast<APawn>( GetOwner() );
	if ( IsValid( Pawn ) && Pawn->IsLocallyControlled() )
	{
		return true;
	}
	return false;
}