// Fill out your copyright notice in the Description page of Project Settings.


#include "EvaluationAbilityClass.h"


float UEvaluationAbilityClass::GetAbilityWeight_Implementation(AActor* AbilityOwner) const
{
	UE_LOG(LogTemp, Warning, TEXT("%s(). This function is not implemented. You should implement it in your child blueprint class"), *FString(__FUNCTION__) );
	return 0.f;
}
