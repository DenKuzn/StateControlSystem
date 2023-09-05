// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "BoxStrategyGameStateBase_Play.h"


// Sets default values
ABoxStrategyGameStateBase_Play::ABoxStrategyGameStateBase_Play()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoxStrategyGameStateBase_Play::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("%s() Started"), *FString(__FUNCTION__));
	
}

// Called every frame
void ABoxStrategyGameStateBase_Play::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
}

