// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyPlayerPawn_Play.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"


// Sets default values
ABoxStrategyPlayerPawn_Play::ABoxStrategyPlayerPawn_Play()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoxStrategyPlayerPawn_Play::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxStrategyPlayerPawn_Play::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ABoxStrategyPlayerPawn_Play::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AStrategyCorePlayerPawn::CameraMove);
		//EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AStrategyCorePlayerPawn::CameraScroll);

	}
}
