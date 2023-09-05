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

void UStateControlInteractComponent::GetStateControlTag(FGameplayTag& OutStateControlTag)
{
	OutStateControlTag = StateControlTag;
}

void UStateControlInteractComponent::ActivateObjectSelection()
{
	bSelected = true;
	ObjectWasSeleceted.Broadcast();
}

void UStateControlInteractComponent::DeactivateObjectSelection()
{
	bSelected = false;
	ObjectWasDeseleceted.Broadcast();
}

