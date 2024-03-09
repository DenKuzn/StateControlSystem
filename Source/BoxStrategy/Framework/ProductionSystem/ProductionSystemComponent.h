// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProductionStage_Base.h"
#include "ProductionSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FProductionSystemSimpleDelegate );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FProductionSystemProgressDelegate, float, NormalizedProgress );


class UBoxStrategyProductData_Base;

/** Complonent that response for produce new products */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOXSTRATEGY_API UProductionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProductionSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UProductionStage_Base> FirstStage = nullptr;

	UPROPERTY()
	TObjectPtr<UBoxStrategyProductData_Base> CurrentProductData = nullptr;

public:
	UPROPERTY( BlueprintAssignable )
	FProductionStageSelfReferenceDelegate OnStageStarted;

	UPROPERTY( BlueprintAssignable )
	FProductionStageSelfReferenceDelegate OnStageCompleted;


	UPROPERTY( BlueprintAssignable )
	FProductionSystemSimpleDelegate OnProductionStarted;

	UPROPERTY( BlueprintAssignable )
	FProductionSystemProgressDelegate OnProductionProgressUpdated;

	UPROPERTY( BlueprintAssignable )
	FProductionSystemSimpleDelegate OnProductionCompleted;


	UPROPERTY( BlueprintAssignable )
	FProductionSystemSimpleDelegate OnProductionCanceled;

public:
	UFUNCTION( BlueprintPure )
	bool CanStartNewProduction() const;

	UFUNCTION( BlueprintPure )
	void GetCurrentStageDescription(FObjectDescription& StageDescription) const;

	UFUNCTION( BlueprintCallable )
	void AddNextProduct(UBoxStrategyProductData_Base* NewProductData);

	UFUNCTION( BlueprintCallable )
	void ActivateProduction();

	UFUNCTION( BlueprintCallable )
	void CancelProduction();

private:
	UFUNCTION()
	void StageActivated(UProductionStage_Base* CurrentStage);

	UFUNCTION()
	void StageCompleted(UProductionStage_Base* CurrentStage);

	UFUNCTION()
	void StageChainComplete();

	UFUNCTION()
	void StageUpdated(UProductionStage_Base* CurrentStage);
};
