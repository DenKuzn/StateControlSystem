// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"

#include "UnitOrderUtilityAIComponent.generated.h"




class UUnitOrderAbilitySystemComponent;
class UUnitOrderSubsystem;

//TODO: Move this class to UnitOrderUnitController.
// New class will represent controllers that can take control of an unit.
// Controllers can work as UtilityAI controllers or Work controllers that can get control of the unit when player is not controlling it.

/** Перенести в новый класс, который будет представлять собой базовый контроллер с базовыми функциями типа - запуск, получение инфы, что абилка запущена из другого типа и так далее.
 *  Класс представляет собой автоматическую систему контроля над юнитами (AI), которая может быть подключена и отключена от юнита и которая не может перебивать работу других контроллеров */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNITORDERSYSTEM_API UUnitOrderUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUnitOrderUtilityAIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


	// Default Unit Order Type:
	UPROPERTY( EditDefaultsOnly, Category = "UnitOrder" )
	FGameplayTag UnitOrderTypeTag;

public:

	UUnitOrderAbilitySystemComponent* GetUnitOrderAbilitySystemComponent();

	// From this function UtilityAI will start working.
	UFUNCTION(BlueprintCallable)
	void ActivateNewBehavior();

private:

	UUnitOrderSubsystem* GetUnitOrderSubsystem();

	//Get Next function tag that has most weight and priority.
	FGameplayTag GetNextAbilityTag(const FGameplayTag& UnitSubtype);

	UFUNCTION()
	void AbilityWasEnded(FGameplayTag ControlTypeTag);

	void FindAndExecuteNewAbility();

};
