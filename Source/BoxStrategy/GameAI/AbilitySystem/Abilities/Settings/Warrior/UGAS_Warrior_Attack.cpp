// Fill out your copyright notice in the Description page of Project Settings.


#include "UGAS_Warrior_Attack.h"

void UUGAS_Warrior_Attack::SetAbilitySettings(AActor* NewTargetActor, bool NeedChaseTarget)
{
	TargetActor = NewTargetActor;
	bNeedChaseTarget = NeedChaseTarget;
}
