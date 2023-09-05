// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoxStrategyPlayerController_Play.generated.h"

UCLASS()
class BOXSTRATEGY_API ABoxStrategyPlayerController_Play : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxStrategyPlayerController_Play();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

};
