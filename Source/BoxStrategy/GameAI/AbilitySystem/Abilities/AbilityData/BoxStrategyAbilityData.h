// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "BoxStrategy/Framework/GameData/Structs/BoxStrategyObjectDescription.h"
#include "BoxStrategyAbilityData.generated.h"


class UStateControl;
class UBoxStrategyAbilitySettings;
class UBoxStrategyGameplayAbility;

/** Структура для хранения данных об абилке и ее теге, а так же описания абилки для использования в виде кнопки в UI.
 *  Структура используется не только для списка абилок, но и может быть использована в предметах */

USTRUCT(BlueprintType, Blueprintable)
struct FBoxStrategyAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBoxStrategyObjectDescription AbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityTags")
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBoxStrategyGameplayAbility> UnitAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeUsedInUI = false;

};