// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyBuilding.h"

#include "BoxStrategy/StateControlSystem/StateControlInteractComponent.h"


// Sets default values
ABoxStrategyBuilding::ABoxStrategyBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("Interaction Component");
}

// Called when the game starts or when spawned
void ABoxStrategyBuilding::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABoxStrategyBuilding::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

UStateControlInteractComponent* ABoxStrategyBuilding::GetInteractComponent()
{
	return InteractComponent;
}

