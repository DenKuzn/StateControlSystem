// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.


#include "StateControl.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/StateControlSystemComponent.h"
#include "StateControlWidget.h"
#include "StateControlDataAsset.h"
#include "Components/StateControlInteractComponent.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "Misc/DataValidation.h"

DEFINE_LOG_CATEGORY_STATIC( StateControlLog, Log, All );

UWorld* UStateControl::GetWorld() const
{
	if ( !IsInstantiated() )
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}
	return GetOuter()->GetWorld();
}

bool UStateControl::StateControlHasInputs()
{
	return InputData && InputData->InputMappingContext;
}

void UStateControl::InitializeStateControl(TSubclassOf<UStateControlWidget> StateWidget)
{
	if ( IsValid( StateWidget ) )
	{
		StateWidgetClass = StateWidget;
	}
	else
	{
		UE_LOG( StateControlLog, Warning, TEXT("%s(). StateWidget invalid."), *FString(__FUNCTION__) );
	}

	// If state have Controls then must initialize it.
	if ( StateControlHasInputs() )
	{
		if ( GetStateControlSystemComponent()->IslocalyControlled() )
		{
			{
#if GAME_DEBUG_BUILDS
				if ( InputData->InputActionData.Num() == 0 )
				{
					ensureAlwaysMsgf( false, TEXT("%s(). InputData->InputActionData.Num() == 0"), *FString(__FUNCTION__) );
				}
#endif
			}

			if ( UEnhancedInputComponent* EnhancedInputComponent = GetInputComponent() )
			{
				for ( auto& InputActionData : InputData->InputActionData )
				{
					EnhancedInputComponent->BindAction( InputActionData.InputAction, ETriggerEvent::Triggered, this, &UStateControl::Input_InputTagPressed, InputActionData.InputTag );
					EnhancedInputComponent->BindAction( InputActionData.InputAction, ETriggerEvent::Completed, this, &UStateControl::Input_InputTagReleased, InputActionData.InputTag );
				}
			}
			else
			{
				ensureAlwaysMsgf( false, TEXT("%s(). EnhancedInputComponent invalid"), *FString(__FUNCTION__) );
			}
		};
	}


	InputTagPropertyMap.Initialize( this );
}

void UStateControl::ActivateHotKeys()
{
	InputTagPropertyMap.ResetAllValuesToZero();

	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( GetStateControlSystemComponent(), );
#endif
	}
	if ( StateControlHasInputs() )
	{
		if ( GetStateControlSystemComponent()->IslocalyControlled() )
		{
			if ( UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
			{
				InputSubsystem->AddMappingContext( InputData->InputMappingContext, 1 );
			}
			else
			{
				ensureAlwaysMsgf( false, TEXT("%s(). InputSubsystem invalid"), *FString(__FUNCTION__) );
			}
		}
	}

}

void UStateControl::ActivateState()
{
	if(GetStateControlStatus() != EStateControlStatus::ME_Deactivated)
	{
		ensureAlwaysMsgf( false, TEXT("%s(). GetStateControlStatus() != EStateControlStatus::ME_Deactivated"), *FString(__FUNCTION__) );
		return;
	}

	StateControlStatus = EStateControlStatus::ME_Activated;
	ActivateHotKeys();
	K2_ActivateState();
}

void UStateControl::DeactivateHotKeys()
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( GetStateControlSystemComponent(), );
#endif
	}

	if ( StateControlHasInputs() )
	{
		if ( GetStateControlSystemComponent()->IslocalyControlled() )
		{
			if ( UEnhancedInputLocalPlayerSubsystem* InputSubsystem = GetWorld()->GetFirstPlayerController()->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() )
			{
				InputSubsystem->RemoveMappingContext( InputData->InputMappingContext );
			}
			else
			{
				ensureAlwaysMsgf( false, TEXT("%s(). InputSubsystem invalid"), *FString(__FUNCTION__) );
			}
		}
	}

	InputTagPropertyMap.ResetAllValuesToZero();
}

void UStateControl::DeactivateState()
{
	if(GetStateControlStatus() == EStateControlStatus::ME_Deactivated)
	{
		ensureAlwaysMsgf( false, TEXT("%s(). GetStateControlStatus() == EStateControlStatus::ME_Deactivated"), *FString(__FUNCTION__) );
		return;
	}

	StateControlStatus = EStateControlStatus::ME_Deactivated;
	DeactivateHotKeys();
	K2_DeactivateState();

}

void UStateControl::SleepState()
{
	if(GetStateControlStatus() != EStateControlStatus::ME_Activated)
	{
		ensureAlwaysMsgf( false, TEXT("%s(). GetStateControlStatus() != EStateControlStatus::ME_Activated"), *FString(__FUNCTION__) );
		return;
	}

	StateControlStatus = EStateControlStatus::ME_Sleeping;
	K2_SleepState();
	DeactivateHotKeys();
}

void UStateControl::WakeUpState()
{
	if(GetStateControlStatus() != EStateControlStatus::ME_Sleeping)
	{
		ensureAlwaysMsgf( false, TEXT("%s(). GetStateControlStatus() != EStateControlStatus::ME_Sleeping"), *FString(__FUNCTION__) );
		return;
	}

	StateControlStatus = EStateControlStatus::ME_Activated;
	K2_WakeUpState();
	ActivateHotKeys();
}

bool UStateControl::IsConsumeActions()
{
	return bConsumeActions;
}

UStateControlInteractComponent* UStateControl::GetInteractComponentWithMyTagUnderMouse(ECollisionChannel TraceChannel, bool bTraceComplex)
{
	if ( UStateControlInteractComponent* InteractComponent = GetInteractComponentUnderMouse(TraceChannel, bTraceComplex) )
	{
		FGameplayTag StateControlTag;
		InteractComponent->GetStateControlTag( StateControlTag );

		if ( IsMyTag( StateControlTag ) )
		{
			return InteractComponent;
		};
	};

	return nullptr;

}

UStateControlInteractComponent* UStateControl::GetInteractComponentUnderMouse(ECollisionChannel TraceChannel, bool bTraceComplex)
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor( TraceChannel, bTraceComplex, HitResult );

	if ( IsValid( HitResult.GetActor() ) )
	{
		return HitResult.GetActor()->FindComponentByClass<UStateControlInteractComponent>();
	};

	return nullptr;
}

FVector UStateControl::GetLocationUnderMouse(bool& bSuccess, ECollisionChannel TraceChannel, bool bTraceComplex)
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor( TraceChannel, bTraceComplex, HitResult );

	if ( HitResult.bBlockingHit )
	{
		bSuccess = true;
		return HitResult.Location;
	}

	bSuccess = false;
	return FVector::ZeroVector;
}

bool UStateControl::IsMyTag(FGameplayTag StateControlTag)
{
	UStateControlSystemComponent* ControlComponent = GetStateControlSystemComponent();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( ControlComponent, false );
#endif
	}

	return ControlComponent->IsTagForStateControl( StateControlTag, this);
}

bool UStateControl::IsActiveStateControl()
{
	UStateControlSystemComponent* ControlComponent = GetStateControlSystemComponent();
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( ControlComponent, false );
#endif
	}

	return ControlComponent->IsStateControlActive( this );
}

EStateControlStatus UStateControl::GetStateControlStatus()
{
	return StateControlStatus;
}

void UStateControl::NewInputTagPropertyMapState(FGameplayTag StateControlInputTag, bool NewState)
{
	{
#if GAME_DEBUG_BUILDS
		if ( !StateControlInputTag.IsValid() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !StateControlInputTag.IsValid()"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	InputTagPropertyMap.GameplayTagEventCallback( StateControlInputTag, NewState );
}

bool UStateControl::TryActivateStateControlFromObjectUnderMouse()
{
	UStateControlInteractComponent* InteractComponent = GetInteractComponentUnderMouse();
	if(!IsValid( InteractComponent ) || !InteractComponent->IsInteractionEnabled())
	{
		return false;
	}

	UStateControlSystemComponent* StateControlSystemComponent = GetStateControlSystemComponent();
	if(!IsValid( StateControlSystemComponent ))
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid( StateControlSystemComponent )"), *FString(__FUNCTION__) );
		return false;
	}

	FGameplayTag StateControlTag;
	InteractComponent->GetStateControlTag(StateControlTag);

	StateControlSystemComponent->SetNewActiveStateByTag( StateControlTag );
	return true;

}

UStateControlSystemComponent* UStateControl::GetStateControlSystemComponent()
{
	return Cast<UStateControlSystemComponent>( GetOuter() );
}

bool UStateControl::IsComponentInteractionEnabled(UStateControlInteractComponent* InteractComponent)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN(InteractComponent, false  );
#endif
	}

	return InteractComponent->IsInteractionEnabled();
}

bool UStateControl::IsInstantiated() const
{
	return !HasAllFlags( RF_ClassDefaultObject );
}

void UStateControl::Input_InputTagPressed(FGameplayTag InputTag)
{
	NewInputTagPropertyMapState( InputTag, true );
	StateControlInputTriggered( InputTag, true );
}

void UStateControl::Input_InputTagReleased(FGameplayTag InputTag)
{
	NewInputTagPropertyMapState( InputTag, false );
	StateControlInputTriggered( InputTag, false );
}

UEnhancedInputComponent* UStateControl::GetInputComponent()
{
	return Cast<UEnhancedInputComponent>( GetWorld()->GetFirstPlayerController()->InputComponent );
}

#if WITH_EDITOR
EDataValidationResult UStateControl::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid( ValidationErrors );

	InputTagPropertyMap.IsDataValid( this, ValidationErrors );

	return ( ( ValidationErrors.Num() > 0 ) ? EDataValidationResult::Invalid : EDataValidationResult::Valid );
}
#endif
