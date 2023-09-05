// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyUnitCharacter.h"

#include "BoxStrategy/StateControlSystem/StateControlInteractComponent.h"


// Sets default values
ABoxStrategyUnitCharacter::ABoxStrategyUnitCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("Interaction Component");
}

// Called when the game starts or when spawned
void ABoxStrategyUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoxStrategyUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ABoxStrategyUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
}

UStateControlInteractComponent* ABoxStrategyUnitCharacter::GetInteractComponent()
{
	return InteractComponent;
}

