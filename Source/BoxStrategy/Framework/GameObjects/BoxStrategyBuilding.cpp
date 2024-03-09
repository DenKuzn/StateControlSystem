// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyBuilding.h"

#include "StateControlSystem/Framework/Components/StateControlInteractComponent.h"


// Sets default values
ABoxStrategyBuilding::ABoxStrategyBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateControlInteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("State Control Interaction Component");
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

void ABoxStrategyBuilding::SetBuildingData(UConstructionData_Base* NewConstructionData)
{
	BoxStrategyConstructionData = NewConstructionData;
}

UStateControlInteractComponent* ABoxStrategyBuilding::GetInteractComponent()
{
	return StateControlInteractComponent;
}

