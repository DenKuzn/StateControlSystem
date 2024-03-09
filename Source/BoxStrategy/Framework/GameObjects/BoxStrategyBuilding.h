// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxStrategyBuilding.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EBuildingState : uint8
{
	SearchingPlace,
	Constructing,
	Working,
	Disabled
};


/** Buildings are objects that can be built on the map.
 */

class UBoxStrategyBuildingData;
class UConstructionData_Base;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EBuildingState BuildingState = EBuildingState::SearchingPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UConstructionData_Base* BoxStrategyConstructionData = nullptr;

public:

	UFUNCTION(BlueprintCallable)
	void SetBuildingData(UConstructionData_Base* NewConstructionData);

	UFUNCTION(BlueprintCallable)
	void SetBuildingState(EBuildingState NewState) { BuildingState = NewState; };

	UFUNCTION(BlueprintPure)
	EBuildingState GetBuildingState() const { return BuildingState; };

	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponent();
};
