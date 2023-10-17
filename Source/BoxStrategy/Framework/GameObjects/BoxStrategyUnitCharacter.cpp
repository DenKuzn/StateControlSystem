// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyUnitCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "BoxStrategy/GameAI/AbilitySystem/BoxStrategyAbilitySystemComponent.h"
#include "BoxStrategy/StateControlSystem/StateControlInteractComponent.h"
#include "BoxStrategy/GeneralDebugMacroses.h"


// Sets default values
ABoxStrategyUnitCharacter::ABoxStrategyUnitCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractComponent = CreateDefaultSubobject<UStateControlInteractComponent>("Interaction Component");

	BoxStrategyUnitAbilitySystemComponent = CreateDefaultSubobject<UBoxStrategyAbilitySystemComponent>("Unit Ability System Component");
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
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( BoxStrategyUnitAbilitySystemComponent, );
#endif
	}

	BoxStrategyUnitAbilitySystemComponent->InitAbilityActorInfo( NewController, this);
	BoxStrategyUnitAbilitySystemComponent->AbilityActorInfo->SkeletalMeshComponent = GetMesh();
}

UStateControlInteractComponent* ABoxStrategyUnitCharacter::GetInteractComponent()
{
	return InteractComponent;
}

AAIController* ABoxStrategyUnitCharacter::GetAIUnitController()
{
	return Cast<AAIController>(GetController());
}

UBoxStrategyAbilitySystemComponent* ABoxStrategyUnitCharacter::GetBoxStrategyAbilitySystemComponent()
{
	return BoxStrategyUnitAbilitySystemComponent;
}

