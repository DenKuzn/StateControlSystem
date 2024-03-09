// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"
#include "BoxStrategyProductData_Base.generated.h"

class UProductionStage_Base;

/** Все объекты в игре, которые должны производиться - должны наследоваться от этого класса.
 *
 */
UCLASS(BlueprintType)
class BOXSTRATEGY_API UBoxStrategyProductData_Base : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FObjectDescription Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MainTypeTag;

	/** Определяют суть этого ресурса.
	 * - Тип ресурса. Производственный, природный.
	 * - Действия, которые можно сделать с этим ресурсом (добывать, искать, готовить и так далее).
	 * - Какой тип слота использования для персонажа.
	 *		- Тип Одежды (штаны, куртка).
	 *		- Тип использования рук (одноручный, двуручный). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer MetaTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Parameters")
	float HealthPoints = 100.f;

	//Количество очков производства, которые требуются для постройки этого здания.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Production")
	int32 ProductionPoints = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Production")
	TMap<UBoxStrategyProductData_Base*, int32> RequestedObjects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Production")
	TArray<TSubclassOf<UProductionStage_Base>> ProductionStages;
};
