// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BoxStrategyGameStateBase_Play.generated.h"


UCLASS()
class BOXSTRATEGY_API ABoxStrategyGameStateBase_Play : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxStrategyGameStateBase_Play();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
