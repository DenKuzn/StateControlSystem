// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyUnitCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"
#include "StateControlSystem/Framework/Components/StateControlInteractComponent.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"


// Sets default values
ABoxStrategyUnitCharacter::ABoxStrategyUnitCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateControlInteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("State Control Interaction Component");
	UnitOrderAbilitySystemComponent = CreateDefaultSubobject<UUnitOrderAbilitySystemComponent>("Unit Ability System Component");
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

void ABoxStrategyUnitCharacter::PossessedBy(AController* NewController)
{
	FUNCTION_START_LOG();

	Super::PossessedBy( NewController );
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( UnitOrderAbilitySystemComponent, );
#endif
	}

	UnitOrderAbilitySystemComponent->InitAbilityActorInfo( NewController, this);
	UnitOrderAbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent = GetMesh();
}

UStateControlInteractComponent* ABoxStrategyUnitCharacter::GetInteractComponent()
{
	return StateControlInteractComponent;
}

AAIController* ABoxStrategyUnitCharacter::GetAIUnitController()
{
	return Cast<AAIController>(GetController());
}

UUnitOrderAbilitySystemComponent* ABoxStrategyUnitCharacter::GetUnitOrderAbilitySystemComponent()
{
	return UnitOrderAbilitySystemComponent;
}

