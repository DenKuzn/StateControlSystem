// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ObjectDescriptionPlugin/Framework/ObjectDescriptionStruct.h"
#include "UnitOrderAbilityData.generated.h"


class UUnitOrderAbilitySettings;
class UUnitOrderGameplayAbility;

/** Структура для хранения данных об абилке и ее теге, а так же описания абилки для использования в виде кнопки в UI.
 *  Структура используется не только для списка абилок, но и может быть использована в предметах */
USTRUCT(BlueprintType, Blueprintable)
struct UNITORDERSYSTEM_API FUnitOrderAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FObjectDescription AbilityDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityTags")
	FGameplayTag AbilityTag;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<UUnitOrderGameplayAbility> UnitAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UUnitOrderGameplayAbility> UnitAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeUsedInUI = false;

};