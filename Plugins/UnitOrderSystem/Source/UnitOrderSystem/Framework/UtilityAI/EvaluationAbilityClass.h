// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EvaluationAbilityClass.generated.h"

/**
 * 
 */
UCLASS()
class UNITORDERSYSTEM_API UEvaluationAbilityClass : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UnitOrderSystem|UtilityAI")
	float GetAbilityWeight(AActor* AbilityOwner) const;

};
