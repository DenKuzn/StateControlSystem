// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyUnitTask.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

UWorld* UBoxStrategyUnitTask::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
	{
		// If we are a CDO, we must return nullptr instead of calling Outer->GetWorld() to fool UObject::ImplementsGetWorld.
		return nullptr;
	}

	if(GetOuter() && GetOuter()->GetWorld())
	{
		return GetOuter()->GetWorld();
	}

	return UObject::GetWorld();
}

EBoxStrategyUnitTaskState UBoxStrategyUnitTask::GetTaskCurrentStatus() const
{
	return TaskCurrentStatus;
}

int32 UBoxStrategyUnitTask::GetTaskPriorityLevel() const
{
	return TaskPriorityLevel;
}

int32 UBoxStrategyUnitTask::GetMaxWorkers() const
{
	return MaxWorkers;
}

const FGameplayTagContainer& UBoxStrategyUnitTask::GetWorkerSubtypeTags() const
{
	return WorkerSubtypeTags;
}

const FGameplayTag& UBoxStrategyUnitTask::GetTaskTypeTag() const
{
	return TaskTypeTag;
}

void UBoxStrategyUnitTask::SetTaskCurrentStatus(EBoxStrategyUnitTaskState NewStatus)
{
	TaskCurrentStatus = NewStatus;
}

void UBoxStrategyUnitTask::SetTaskPriorityLevel(int32 NewPriorityLevel)
{
	TaskPriorityLevel = NewPriorityLevel;
}

void UBoxStrategyUnitTask::AddNewWorker(UObject* NewWorker)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewWorker, );

		if(AssignedWrokers.Contains( NewWorker ))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). Worker is already added. Worker Name: %s"), *FString(__FUNCTION__), *NewWorker->GetName());
			return;
		}
#endif
	}

	AssignedWrokers.AddUnique( NewWorker );
}

void UBoxStrategyUnitTask::RemoveWorker(UObject* WorkerToRemove)
{
	AssignedWrokers.Remove( WorkerToRemove );
}

void UBoxStrategyUnitTask::ClearAllWorkers()
{
	AssignedWrokers.Empty();
}

int32 UBoxStrategyUnitTask::GetCurrentWorkersNum() const
{
	return AssignedWrokers.Num();
}

bool UBoxStrategyUnitTask::CanUnitWorkOnThisTask_Implementation(UObject* TaskSearchingUnit)
{
	ensureAlwaysMsgf( false, TEXT("%s(). This function must be overrided. Object: %s"), *FString(__FUNCTION__), *GetName() );
	return false;
}

void UBoxStrategyUnitTask::TryAssignUnitsToTask_Implementation(const TArray<UObject*>& UnitsToAssign)
{
	ensureAlwaysMsgf( false, TEXT("%s(). This function must be overrided. Object: %s"), *FString(__FUNCTION__), *GetName() );
}