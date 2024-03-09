// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "TaskManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FTaskManagerSimpleDelegate );


class UBoxStrategyUnitTask;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class BOXSTRATEGY_API UTaskManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTaskManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UBoxStrategyUnitTask*> ActiveTasks;

	UPROPERTY()
	TArray<UObject*> AvailableForWorkObjects;

public:
	// Для дебага больше.
	UFUNCTION( BlueprintPure, Category = "TaskManager" )
	const TArray<UBoxStrategyUnitTask*>& GetActiveTasks();

	// Для дебага больше.
	UFUNCTION( BlueprintPure, Category = "TaskManager" )
	TArray<UObject*>& GetAvailableForWorkObjects();

	// Проверяет базово - можно ли юниту запуститься в работу с этой таской.
	UFUNCTION( BlueprintCallable, Category = "TaskManager" )
	bool TryAssignTaskToUnit(const UObject* ForObjectWithInterface);

	UFUNCTION( BlueprintCallable, Category = "TaskManager" )
	void AddForTaskWaitingObject(UObject* NewObjectWithInterface);

	UFUNCTION( BlueprintCallable, Category = "TaskManager" )
	void RemoveTaskWaitingObject(UObject* RemovableObject);

	UFUNCTION( BlueprintCallable )
	void AddNewTask(UBoxStrategyUnitTask* NewTask);

	UFUNCTION( BlueprintCallable )
	void RemoveTask(UBoxStrategyUnitTask* ExistingTask);

private:
	void ValidateTasks();

	void ValidateObjects();
};
