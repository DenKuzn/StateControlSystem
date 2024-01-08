// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxStrategyBuilding.generated.h"


/** Buildings are objects that can be built on the map.
 */

UCLASS()
class BOXSTRATEGY_API ABoxStrategyBuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxStrategyBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlInteractComponent> StateControlInteractComponent = nullptr;

public:

	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponent();
};
