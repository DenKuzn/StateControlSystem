// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"
#include "ProductionStage_Base.generated.h"

class UProductionStage_Base;
class UBoxStrategyProductData_Base;


//TODO: Добавить возможность запускать каждый этап вручную, чтобы контролировать процесс еще более тонко.

UENUM(Blueprintable, BlueprintType)
enum class EProductionStageState : uint8
{
	WaitActivation,
	Active,
	Finished,
	Canceled
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FProductionStageSimpleDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FProductionStageSelfReferenceDelegate, UProductionStage_Base*, CurrentStage );

/** рекурсивный класс для стадий производства */
UCLASS(Blueprintable, BlueprintType)
class BOXSTRATEGY_API UProductionStage_Base : public UObject
{
	GENERATED_BODY()

public:

	virtual UWorld* GetWorld() const override;

private:

	UPROPERTY()
	UProductionStage_Base* NextProductionStage = nullptr;

	UPROPERTY()
	UBoxStrategyProductData_Base* ProductionData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Production", meta = (AllowPrivateAccess = "true"))
	EProductionStageState ProductionStageState = EProductionStageState::WaitActivation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Production", meta = (AllowPrivateAccess = "true"))
	FObjectDescription StageDescription;

public:

	UPROPERTY(BlueprintAssignable)
	FProductionStageSelfReferenceDelegate OnStageActivated;

	UPROPERTY(BlueprintAssignable)
	FProductionStageSelfReferenceDelegate OnStageCompleted;

	UPROPERTY(BlueprintAssignable)
	FProductionStageSimpleDelegate OnStageChainComplete;


	UPROPERTY(BlueprintAssignable)
	FProductionStageSimpleDelegate OnStageCanceled;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FProductionStageSelfReferenceDelegate OnStageUpdated;

public:

	UFUNCTION(BlueprintCallable)
	void InitializationStage(UBoxStrategyProductData_Base* NewProductData);

	UFUNCTION(BlueprintCallable)
	void SetNextStage(UProductionStage_Base* NewNextStage);

	UFUNCTION(BlueprintCallable)
	void ExecuteProduction();

	UFUNCTION(BlueprintCallable)
	void StopProductionExecution();

	UFUNCTION(BlueprintCallable)
	void SetNewPriority(int32 NewPriority);

	UFUNCTION(BlueprintPure, Category="Production")
	UBoxStrategyProductData_Base* GetProductionData() { return ProductionData; }

	UFUNCTION(BlueprintPure, Category="Production")
	EProductionStageState GetProductionStageState() { return ProductionStageState; }

	UFUNCTION(BlueprintPure, Category="Production")
	AActor* GetOwnerActor();

	UFUNCTION(BlueprintNativeEvent)
	float GetStageProgress();

	//Внимание. Вызывать эту функцию можно только из первой стадии в цепочке.
	// Если вызвать из второй стадии, а запущена первая, то будет nullptr.
	UFUNCTION(BlueprintPure, Category="Production")
	UProductionStage_Base* GetCurrentActiveStage();

	UFUNCTION(BlueprintPure, Category="Production")
	const FObjectDescription& GetStageDescription() const { return StageDescription; }

	UFUNCTION(BlueprintNativeEvent, Category="Production")
	FText GetStageRequirements();


public:
/// DEBUG FUNCTIONS

	// Возвращает количество стадий в цепочке от текущей стадии.
	// Если есть стадии выше, то они не будут учитываться и в счетчике будет только текущая стадия и все стадии ниже.
	UFUNCTION(BlueprintPure)
	int32 GetStageTopID();

	UFUNCTION(BlueprintPure)
	void GetAllStageStates(TArray<EProductionStageState>& OutStates);

protected:


	UFUNCTION(BlueprintImplementableEvent, Category = "ProductionExecution", DisplayName = "Activate Production Execution", meta=(ScriptName = "InternalActivateProductionExecution"))
	void K2_ActivateExecution();


	/** Вызывается при отмене выполнения всей цепочки стадий.
	 * Внутри этого метода должны быть отменены все действия, которые были начаты внутри этой стадии даже если они завершены.
	 * Например, этап строительства здания "принести ресурс" был завершен, но страительство отменилось на этапе стройки, тогда этот этап должен отменить принесение ресурса. */
	UFUNCTION(BlueprintImplementableEvent, Category = "ProductionExecution", DisplayName = "Cancel Production Execution", meta=(ScriptName = "InternalCancelProductionExecution"))
	void K2_CancelExecution();


protected:

	UFUNCTION(BlueprintCallable)
	void CurrentStageCompleted();

private:

	UFUNCTION()
	void OnLastStageInChainCompleted();

	UFUNCTION()
	void OnNextStageCanceled();

	UFUNCTION()
	void OnNewProductionStageActivated(UProductionStage_Base* NewProductionStageActivated);

	UFUNCTION()
	void OnNewProductionStageCompleted(UProductionStage_Base* NewProductionStageCompleted);

	UFUNCTION()
	void OnNextStageUpdated(UProductionStage_Base* CurrentStage);

};
