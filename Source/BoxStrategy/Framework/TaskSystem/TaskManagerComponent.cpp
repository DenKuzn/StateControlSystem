// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskManagerComponent.h"

#include "BoxStrategyUnitTask.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "SearchingWorkObjectInterface.h"


// Sets default values for this component's properties
UTaskManagerComponent::UTaskManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTaskManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

const TArray<UBoxStrategyUnitTask*>& UTaskManagerComponent::GetActiveTasks()
{
	ValidateTasks();
	return ActiveTasks;
}

TArray<UObject*>& UTaskManagerComponent::GetAvailableForWorkObjects()
{
	ValidateObjects();
	return AvailableForWorkObjects;
}

bool UTaskManagerComponent::TryAssignTaskToUnit(const UObject* ForObjectWithInterface)
{
	const TArray<UBoxStrategyUnitTask*>& Checked_ActiveTasks = GetActiveTasks();

	if ( Checked_ActiveTasks.IsEmpty() )
	{
		return false;
	}

	{
#if GAME_DEBUG_BUILDS

		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( ForObjectWithInterface, false );

		if ( !ForObjectWithInterface->Implements<USearchingWorkObjectInterface>() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). NewWaitingTaskObject->Implements<ISearchingWorkObjectInterface>() == false"), *FString(__FUNCTION__) );
			return false;
		}
#endif
	}

	UBoxStrategyUnitTask* SelectedTask = nullptr;

	// Ищем доступные задачи для этого юнита по приоритету задач.
	for ( auto* Task : Checked_ActiveTasks )
	{
		if ( Task->GetWorkerSubtypeTags().HasTagExact( ISearchingWorkObjectInterface::Execute_GetUnitSubtypeTag( ForObjectWithInterface ) )
			&&
			Task->CanUnitWorkOnThisTask( ISearchingWorkObjectInterface::Execute_GetOwningActor( ForObjectWithInterface ) ) )
		{
			if ( !IsValid( SelectedTask ) )
			{
				SelectedTask = Task;
				continue;
			}

			if ( IsValid( SelectedTask ) && ( SelectedTask->GetTaskPriorityLevel() < Task->GetTaskPriorityLevel() ) )
			{
				SelectedTask = Task;
			}
		}
	}

	// Если задача найдена - Активируем ее в объекте через интерфейс.
	if ( IsValid( SelectedTask ) )
	{
		ISearchingWorkObjectInterface::Execute_NewTaskAssigned( ForObjectWithInterface, SelectedTask );
		return true;
	}

	return false;
}

void UTaskManagerComponent::AddForTaskWaitingObject(UObject* NewObjectWithInterface)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewObjectWithInterface, );
#endif
	}

	if ( !NewObjectWithInterface->Implements<USearchingWorkObjectInterface>() )
	{
		ensureAlwaysMsgf( false, TEXT("%s(). NewWaitingTaskObject->Implements<ISearchingWorkObjectInterface>() == false"), *FString(__FUNCTION__) );
		return;
	}

	GetAvailableForWorkObjects().AddUnique( NewObjectWithInterface );
}

void UTaskManagerComponent::RemoveTaskWaitingObject(UObject* RemovableObject)
{
	GetAvailableForWorkObjects().Remove( RemovableObject );
}

void UTaskManagerComponent::AddNewTask(UBoxStrategyUnitTask* NewTask)
{
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewTask, );
#endif
	}

	ActiveTasks.AddUnique( NewTask );
	TArray<UObject*> AvailableForTaskUnits;
	AvailableForTaskUnits.Reset( GetAvailableForWorkObjects().Num() );

	for ( auto* TaskSearchingObject : GetAvailableForWorkObjects() )
	{
		if ( TaskSearchingObject->Implements<USearchingWorkObjectInterface>() )
		{
			if ( NewTask->GetWorkerSubtypeTags().HasTagExact( ISearchingWorkObjectInterface::Execute_GetUnitSubtypeTag( TaskSearchingObject ) ) )
			{
				AvailableForTaskUnits.Add( TaskSearchingObject );
			}
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT("%s(). PIZDA"), *FString(__FUNCTION__) );
			//ensureAlwaysMsgf( false, TEXT("%s(). TaskSearchingObject->Implements<ISearchingWorkObjectInterface>() == false"), *FString(__FUNCTION__) );
		}
	}

	NewTask->TryAssignUnitsToTask( AvailableForTaskUnits );
}

void UTaskManagerComponent::RemoveTask(UBoxStrategyUnitTask* ExistingTask)
{
	ActiveTasks.Remove( ExistingTask );
}

void UTaskManagerComponent::ValidateTasks()
{
#if GAME_DEBUG_BUILDS

	TArray<UBoxStrategyUnitTask*> Copy_ActiveTasks = ActiveTasks;
	for ( auto& Task : Copy_ActiveTasks )
	{
		if ( !IsValid( Task ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(Task)"), *FString(__FUNCTION__) );
			ActiveTasks.Remove( Task );
			continue;
		}

		if ( Task->GetTaskCurrentStatus() != EBoxStrategyUnitTaskState::Active )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). Task->GetTaskCurrentStatus() != EBoxStrategyUnitTaskState::Active"), *FString(__FUNCTION__) );
			ActiveTasks.Remove( Task );
			continue;
		}
	}

#endif
}

void UTaskManagerComponent::ValidateObjects()
{
#if GAME_DEBUG_BUILDS

	TArray<UObject*> Copy_AvailableforWorkObjects = AvailableForWorkObjects;

	for ( auto& Object : Copy_AvailableforWorkObjects )
	{
		if ( !IsValid( Object ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(Object)"), *FString(__FUNCTION__) );
			AvailableForWorkObjects.Remove( Object );
			continue;
		}

		if ( !Object->Implements<USearchingWorkObjectInterface>() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !Object->Implements<USearchingWorkObjectInterface>()"), *FString(__FUNCTION__) );
			AvailableForWorkObjects.Remove( Object );
			continue;
		}
	}

#endif
}
