// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "SearchingWorkObjectInterface.generated.h"

class UBoxStrategyUnitTask;
// Этот интерфейс натягивается на любой объект, который хочет записаться в очередь и ожидать выделенной задачи.
UINTERFACE(Blueprintable)
class USearchingWorkObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class BOXSTRATEGY_API ISearchingWorkObjectInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SearchingWorkObject")
	FGameplayTag GetUnitSubtypeTag() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SearchingWorkObject")
	AActor* GetOwningActor() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "SearchingWorkObject")
	void NewTaskAssigned(UBoxStrategyUnitTask* NewTask) const;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
