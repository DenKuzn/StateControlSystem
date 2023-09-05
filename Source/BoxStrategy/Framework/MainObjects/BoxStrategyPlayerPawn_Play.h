// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoxStrategyPlayerPawn_Play.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;



UCLASS()
class BOXSTRATEGY_API ABoxStrategyPlayerPawn_Play : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoxStrategyPlayerPawn_Play();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	
};
