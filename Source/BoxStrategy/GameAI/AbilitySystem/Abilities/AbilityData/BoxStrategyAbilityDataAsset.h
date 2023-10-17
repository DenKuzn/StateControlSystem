// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "BoxStrategyAbilityData.h"
#include "BoxStrategyAbilityDataAsset.generated.h"

/** DA собирается для каждого типа юнита со своим тегом и набором способностей.
 */

struct FBoxStrategyObjectDescription;
struct FBoxStrategyAbilityData;
class UBoxStrategyGameplayAbility;



/** DA которое собирает в себе все возможные абилки для этого типа юнита (не подтипов). Здесь хранятся CDO абилок, описания, иконки и так далее
 *  Хранится 1 CDO в GAS на протяжении всей жизни юнита, так как это список всех возможных действий юнита вообще.
 *  Например. Юнит человек - может быть 3 подтипов:
 *		- Воин.
 *			- Стрелять.
 *			- Идти.
 *		- Строитель.
 *			- Строить.
 *			- Идти.
 *		- Торговец.
 *			- Торговать.
 *			- Идти.
 *
 *	Это DA будет хранить в себе все абилки - идти, атаковать, строить, торговать.
 *	Когда сменяется подтип юнита - то сменяются его активные абилки, но UBoxStrategyAbilityDataAsset всегда остается один и не меняется.
 *	Удобно тем, что мы всегда можем получить из Юнита его список абилок и получить их и описания, и иконки и прочее.
 */
UCLASS()
class BOXSTRATEGY_API UBoxStrategyAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FBoxStrategyAbilityData> AllUnitAbilities;
};


/** DA, которое описывает подтип юнита и его абилки. Для каждого типа юнита свой тег, свои абилки по умолчанию, свои абилки вообще.
 *	Так же хранит прямое описание типа юнита FBoxStrategyObjectDescription.
 *	Все абилки перечисляются Только в виде тегов. */
UCLASS()
class BOXSTRATEGY_API UBoxStrategyUnitAbilityList : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag UnitTypeTag;

	UPROPERTY(EditDefaultsOnly)
	FBoxStrategyObjectDescription UnitTypeDescription;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag DefaultAbilityTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AutoAbilityTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> AbilityTagList;
};