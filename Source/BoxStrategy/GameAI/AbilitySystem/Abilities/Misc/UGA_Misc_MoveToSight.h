// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "BoxStrategy/GameAI/AbilitySystem/Abilities/BoxStrategyGameplayAbility.h"
#include "UGA_Misc_MoveToSight.generated.h"

class UAITask_MoveTo;
/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UUGA_Misc_MoveToSight : public UBoxStrategyGameplayAbility
{
	GENERATED_BODY()

public:
	UUGA_Misc_MoveToSight(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

private:

	UPROPERTY()
	AActor* TargetActor = nullptr;

	float MinimalDistance = -1.f;

	FTimerHandle CheckVisibilityTimerHande;

	FAIRequestID PreviousRequestID = FAIRequestID::InvalidRequest;

private:

	UFUNCTION()
	void MovementCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	void CheckVisibilityAndDistance();
};
