// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "StateControlSystemComponent.h"

#include "BoxStrategy/GeneralDebugMacroses.h"
#include "StateControlDataAsset.h"
#include "StateControlWidget.h"


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
	{
		if(APawn* Player = Cast<APawn>(GetOwner()))
		{
			if(!Player->IsLocallyControlled())
			{
				return;
			}
		}
		else
		{
			if(GetWorld() && GetWorld()->GetFirstPlayerController())
			{
				if(!GetWorld()->GetFirstPlayerController()->IsLocalController())
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


	//check BaseStateControl.
	{
#if !UE_BUILD_SHIPPING
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(BaseStateControlData,);
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(BaseStateControlData->StateControlClass.Get(),);
		if(!BaseStateControlData->StateControlTag.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). BaseStateControl->StateControlTag invalid"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	UStateControl* NewBaseStateControl = NewObject<UStateControl>( this, BaseStateControlData->StateControlClass );
	NewBaseStateControl->InitializeStateControl( BaseStateControlData->StateWidgetClass );

	StateControlsStorage.Add( BaseStateControlData->StateControlTag, NewBaseStateControl );
	CurrentActiveStates.Add( NewBaseStateControl );

	for(auto* StateControlData : AdditionalStateControlData)
	{
		//check data.
		{
#if !UE_BUILD_SHIPPING
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(StateControlData,);
			GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(StateControlData->StateControlClass.Get(),);
			if(!StateControlData->StateControlTag.IsValid())
			{
				ensureAlwaysMsgf( false, TEXT("%s(). StateControlData->StateControlTag invalid"), *FString(__FUNCTION__) );
				return;
			}
#endif

		}
		UStateControl* NewStateControl = NewObject<UStateControl>( this, StateControlData->StateControlClass );
		NewStateControl->InitializeStateControl( StateControlData->StateWidgetClass );
		StateControlsStorage.Add( StateControlData->StateControlTag, NewStateControl );
	}
	// ...

	NewBaseStateControl->ActivateState();
}


// Called every frame
void UStateControlSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UStateControlSystemComponent::ActivateNewStateByTag(FGameplayTag NewStateTag)
{
	{
#if !UE_BUILD_SHIPPING
		if(!NewStateTag.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !NewStateTag.IsValid()"), *FString(__FUNCTION__));
			return;
		}
#endif
	}

	UStateControl** StageInStorage = StateControlsStorage.Find( NewStateTag );
	{
#if !UE_BUILD_SHIPPING
		if ( !IsValid( *StageInStorage ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). StateControl Invalid by tag: %s."), *FString(__FUNCTION__), *NewStateTag.GetTagName().ToString() );
			return;
		}
#endif
	}

	for(auto StateControlElem : CurrentActiveStates)
	{
		if((*StageInStorage) == StateControlElem)
		{
			ensureAlwaysMsgf( false, TEXT("%s(). Find Same Active State. State name: %s"), *FString(__FUNCTION__), *StateControlElem->GetName() );
			return;
		}
	}

	CurrentActiveStates.Add( (*StageInStorage) );

	(*StageInStorage)->ActivateState();
}

void UStateControlSystemComponent::DeactivateState(UStateControl* SearchedStateControl)
{
	//Checker.
	{
#if !UE_BUILD_SHIPPING
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( SearchedStateControl, )
#endif
	}
	CurrentActiveStates.Remove( SearchedStateControl );
	SearchedStateControl->DeactivateState();
}

void UStateControlSystemComponent::DeactivateStateByTag(FGameplayTag SearchedStateTag)
{
	{
#if !UE_BUILD_SHIPPING
		if(!SearchedStateTag.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !NewStateTag.IsValid()"), *FString(__FUNCTION__));
			return;
		}
#endif
	}

	UStateControl** StageInStorage = StateControlsStorage.Find( SearchedStateTag );
	{
#if !UE_BUILD_SHIPPING
		if ( !IsValid( *StageInStorage ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). StateControl Invalid by tag: %s."), *FString(__FUNCTION__), *SearchedStateTag.GetTagName().ToString() );
			return;
		}
#endif
	}

	DeactivateState( ( *StageInStorage ) );
}

bool UStateControlSystemComponent::IsTagForStateControl(FGameplayTag SearchedStateTag, UStateControl* SearchedStateControl)
{
	{
#if !UE_BUILD_SHIPPING
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( SearchedStateControl, false );
		if(!SearchedStateTag.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !SearchedStateTag.IsValid()"), *FString(__FUNCTION__));
			return false;
		}
#endif
	}

	return (*StateControlsStorage.Find( SearchedStateTag )) == SearchedStateControl;
}

void UStateControlSystemComponent::ActivateMainAction()
{


	// Last in array - activate.
	int32 CurrentStatesNum = CurrentActiveStates.Num();
	for(int32 CurrentIndex = CurrentStatesNum - 1; CurrentIndex >= 0; CurrentIndex--)
	{
		// Check instance.
		{
#if !UE_BUILD_SHIPPING
			if(!IsValid(CurrentActiveStates[CurrentIndex]))
			{
				ensureAlwaysMsgf( false, TEXT("%s(). CurrentActiveStates[%i] invalid"), *FString(__FUNCTION__ ), CurrentIndex);
				continue;
			}
#endif
		}

		// We cache this value, because CurrentActiveState might be removed during ActivateMainAction().
		bool bConsumeActions = CurrentActiveStates[CurrentIndex]->IsConsumeActions();

		CurrentActiveStates[CurrentIndex]->ActivateMainAction();
		if(bConsumeActions)
		{
			break;
		}
	}

}

void UStateControlSystemComponent::ActivateBackAction()
{
	// Last in array - activate.
	int32 CurrentStatesNum = CurrentActiveStates.Num();
	for(int32 CurrentIndex = CurrentStatesNum - 1; CurrentIndex >= 0; CurrentIndex--)
	{
		// Check instance.
		{
#if !UE_BUILD_SHIPPING
			if(!IsValid(CurrentActiveStates[CurrentIndex]))
			{
				ensureAlwaysMsgf( false, TEXT("%s(). CurrentActiveStates[%i] invalid"), *FString(__FUNCTION__ ), CurrentIndex);
				continue;
			}
#endif
		}

		// We cache this value, because CurrentActiveState might be removed during ActivateBackAction().
		bool bConsumeActions = CurrentActiveStates[CurrentIndex]->IsConsumeActions();

		CurrentActiveStates[CurrentIndex]->ActivateBackAction();
		if(bConsumeActions)
		{
			break;
		}
	}
}

void UStateControlSystemComponent::ActivateSecondAction()
{
	// Last in array - activate.
	int32 CurrentStatesNum = CurrentActiveStates.Num();
	for(int32 CurrentIndex = CurrentStatesNum - 1; CurrentIndex >= 0; CurrentIndex--)
	{
		// Check instance.
		{
#if !UE_BUILD_SHIPPING
			if(!IsValid(CurrentActiveStates[CurrentIndex]))
			{
				ensureAlwaysMsgf( false, TEXT("%s(). CurrentActiveStates[%i] invalid"), *FString(__FUNCTION__ ), CurrentIndex);
				continue;
			}
#endif
		}
		// We cache this value, because ActivateSecondAction might be removed during ActivateBackAction().
		bool bConsumeActions = CurrentActiveStates[CurrentIndex]->IsConsumeActions();

		CurrentActiveStates[CurrentIndex]->ActivateSecondAction();
		if(bConsumeActions)
		{
			break;
		}
	}
}

bool UStateControlSystemComponent::IsStateControlActive(UStateControl* StateControl)
{
	for(auto* StateControlElement : CurrentActiveStates)
	{
		if(StateControlElement == StateControl)
		{
			return true;
		}
	}

	return false;
}

bool UStateControlSystemComponent::IslocalyControlled()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if(IsValid(Pawn) && Pawn->IsLocallyControlled())
	{
		return true;
	}
	return false;
}

