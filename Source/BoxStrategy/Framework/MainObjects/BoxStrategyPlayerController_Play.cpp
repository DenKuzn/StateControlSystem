// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyPlayerController_Play.h"


// Sets default values
ABoxStrategyPlayerController_Play::ABoxStrategyPlayerController_Play()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
}

// Called when the game starts or when spawned
void ABoxStrategyPlayerController_Play::BeginPlay()
{
	Super::BeginPlay();

	
/*
	FInputModeGameAndUI	GameAndUIMode;
	GameAndUIMode.SetHideCursorDuringCapture( false );
	GameAndUIMode.SetLockMouseToViewportBehavior( EMouseLockMode::DoNotLock );
	SetInputMode( GameAndUIMode );
*/
	
}

// Called every frame
void ABoxStrategyPlayerController_Play::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

