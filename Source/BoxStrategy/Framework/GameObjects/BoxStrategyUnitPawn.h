// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoxStrategyUnitPawn.generated.h"

class UStateControlInteractComponent;

UCLASS()
class BOXSTRATEGY_API ABoxStrategyUnitPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoxStrategyUnitPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlInteractComponent> InteractComponent = nullptr;

public:
	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponent();
};
