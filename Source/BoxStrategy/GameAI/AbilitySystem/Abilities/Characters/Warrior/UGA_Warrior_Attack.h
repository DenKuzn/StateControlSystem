// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "UnitOrderSystem/Framework/Abilities/UnitOrderGameplayAbility.h"
#include "UGA_Warrior_Attack.generated.h"

class UBoxStrategyHandWeaponComponent;
/** Атака по цели с возможностью преследования цели.
 *
 */
UCLASS()
class BOXSTRATEGY_API UUGA_Warrior_Attack : public UUnitOrderGameplayAbility
{
	GENERATED_BODY()

public:

	UUGA_Warrior_Attack(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	void MoveToTarget();

private:

	UPROPERTY()
	UBoxStrategyHandWeaponComponent* WeaponSystemComponent = nullptr;

	UPROPERTY()
	AActor* TargetActor = nullptr;

	bool bNeedChaseTarget = false;

private:

	UFUNCTION()
	void HandleNextAttack();

	UFUNCTION()
	void StartReloadWeapon();

	void BindAllWeaponDelegates();

	void UnbindAllWeaponDelegates();

};
