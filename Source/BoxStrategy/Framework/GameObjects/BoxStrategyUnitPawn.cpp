// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyUnitPawn.h"

#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"
#include "StateControlSystem/Framework/Components/StateControlInteractComponent.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"


// Sets default values
ABoxStrategyUnitPawn::ABoxStrategyUnitPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StateControlInteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("State Control Interaction Component");
}

// Called when the game starts or when spawned
void ABoxStrategyUnitPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoxStrategyUnitPawn::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ABoxStrategyUnitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
}


UStateControlInteractComponent* ABoxStrategyUnitPawn::GetInteractComponent()
{
	return StateControlInteractComponent;
}


