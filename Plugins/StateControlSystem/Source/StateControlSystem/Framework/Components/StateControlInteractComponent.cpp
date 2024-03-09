// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "StateControlInteractComponent.h"


// Sets default values for this component's properties
UStateControlInteractComponent::UStateControlInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStateControlInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UStateControlInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UStateControlInteractComponent::CancelActiveSelection()
{
	if(bSelected)
	{
		OnCancelActiveSelection.Broadcast();
		return;
	}

	ensureAlwaysMsgf( false, TEXT("%S(). Object Called Canccel Selection but was not selected"), *FString(__FUNCTION__) );
}

void UStateControlInteractComponent::GetStateControlTag(FGameplayTag& OutStateControlTag)
{
	OutStateControlTag = StateControlTag;
}

void UStateControlInteractComponent::ChangeStateControlTag(FGameplayTag& NewStateControlTag)
{
	StateControlTag = NewStateControlTag;
	StateTagWasChanged.Broadcast(this);
}

bool UStateControlInteractComponent::ActivateObjectSelection()
{
	if(!bInteractionEnabled)
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !bInteractionEnabled"), *FString(__FUNCTION__) );
		return false;
	}

	if ( bSelected )
	{
		UE_LOG(LogTemp, Warning, TEXT("%s(). Already Selected"), *FString(__FUNCTION__) );
		return true;
	}

	bSelected = true;
	ObjectWasSeleceted.Broadcast();
	return true;
}

void UStateControlInteractComponent::DeactivateObjectSelection()
{
	if(!bSelected)
	{
		return;
	}

	bSelected = false;
	ObjectWasDeseleceted.Broadcast();
}

void UStateControlInteractComponent::SetInteraction(bool bIsInteractionEnabled)
{
	bInteractionEnabled = bIsInteractionEnabled;
	InteractComponentStateWasChanged.Broadcast(this);

	if(bSelected)
	{
		DeactivateObjectSelection();
	}
}

