// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "BoxStrategyUnitTask.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EBoxStrategyUnitTaskState : uint8
{
	Active,
	Completed,
	Canceled
};

/** Часть функционала для работы с Тасками осталась в Blueprints. Не вижу смысла переносить это в С++ сейчас. */
UCLASS(Blueprintable, BlueprintType)
class BOXSTRATEGY_API UBoxStrategyUnitTask : public UObject
{
	GENERATED_BODY()

	// Needs for call functions from Blueprint Libraries.
	virtual UWorld* GetWorld() const override;

private:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TaskTypeTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer WorkerSubtypeTags;

	UPROPERTY()
	int32 TaskPriorityLevel = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxWorkers = 10;

	UPROPERTY()
	TArray<UObject*> AssignedWrokers;

	UPROPERTY()
	EBoxStrategyUnitTaskState TaskCurrentStatus = EBoxStrategyUnitTaskState::Active;



public:

	// Данная функция может использовать любой тип проверки и условий - будь-то класс или своя простая схема.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Task")
	bool CanUnitWorkOnThisTask(UObject* TaskSearchingUnit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Task")
	void TryAssignUnitsToTask(const TArray<UObject*>& UnitsToAssign);

	UFUNCTION(BlueprintPure)
	EBoxStrategyUnitTaskState GetTaskCurrentStatus() const;

	UFUNCTION(BlueprintPure)
	int32 GetTaskPriorityLevel() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxWorkers() const;

	UFUNCTION(BlueprintPure)
	const FGameplayTagContainer& GetWorkerSubtypeTags() const;

	UFUNCTION(BlueprintPure)
	const FGameplayTag& GetTaskTypeTag() const;

	UFUNCTION(BlueprintCallable)
	void SetTaskCurrentStatus(EBoxStrategyUnitTaskState NewStatus);

	UFUNCTION(BlueprintCallable)
	void SetTaskPriorityLevel(int32 NewPriorityLevel);

	UFUNCTION(BlueprintPure)
	int32 GetCurrentWorkersNum() const;

	UFUNCTION(BlueprintCallable)
	void AddNewWorker(UObject* NewWorker);

	UFUNCTION(BlueprintCallable)
	void RemoveWorker(UObject* WorkerToRemove);

	UFUNCTION(BlueprintCallable)
	void ClearAllWorkers();

};
