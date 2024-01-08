// Fill out your copyright notice in the Description page of Project Settings.


#include "UGA_Misc_MoveToSight.h"

#include "AIController.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"
#include "BoxStrategy/GameAI/AbilitySystem/AbilitySettings/Misc/UGAS_Misc_MoveToSight.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

UUGA_Misc_MoveToSight::UUGA_Misc_MoveToSight(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUGA_Misc_MoveToSight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	UUnitOrderAbilitySystemComponent* AbilityComponent = GetUnitOrderAbilitySystemComponent();
	const UUGAS_Misc_MoveToSight* AbilitySettings = Cast<UUGAS_Misc_MoveToSight>(GetMyAbilitySettings());
	{
#if GAME_DEBUG_BUILDS
		if(!IsValid(AbilityComponent))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilityComponent)"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		};

		if(!IsValid(AbilitySettings) || !IsValid(AbilitySettings->GetTargetActor()))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilitySettings) || !IsValid(AbilitySettings->GetTargetActor())"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		}

		if(!ActorInfo || !ActorInfo->AvatarActor.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !ActorInfo || !ActorInfo->AvatarActor.IsValid()"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		}
#endif
	}

	ACharacter* UnitCharacter = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	{
#if GAME_DEBUG_BUILDS
		if(!IsValid(UnitCharacter))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(Character)"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		};
#endif
	}

	AAIController* AIController = Cast<AAIController>(UnitCharacter->GetController());
	{
#if GAME_DEBUG_BUILDS
		if(!IsValid(AIController))
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AIController)"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		};
#endif
	}

	TargetActor = AbilitySettings->GetTargetActor();
	MinimalDistance = AbilitySettings->GetMinimalDistance();
	PreviousRequestID = AIController->GetCurrentMoveRequestID();

	EPathFollowingRequestResult::Type Request = AIController->MoveToActor(AbilitySettings->GetTargetActor(), -1.f, true, true, false);
	AIController->ReceiveMoveCompleted.AddDynamic( this, &UUGA_Misc_MoveToSight::MovementCompleted );

	// Таймер на каждый кадр не получится собрать, что очень печально.
	GetWorld()->GetTimerManager().SetTimer(CheckVisibilityTimerHande, this, &UUGA_Misc_MoveToSight::CheckVisibilityAndDistance, 0.1f, true);
}

void UUGA_Misc_MoveToSight::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       bool bReplicateEndAbility, bool bWasCancelled)
{
	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if ( !IsValid( Character ) )
	{
		return;
	};


	AAIController* AIController = Cast<AAIController>(Character->GetController());
	if ( !IsValid( AIController ) )
	{
		return;
	}


	GetWorld()->GetTimerManager().ClearTimer(CheckVisibilityTimerHande);

	AIController->ReceiveMoveCompleted.RemoveDynamic( this, &UUGA_Misc_MoveToSight::MovementCompleted );
	AIController->StopMovement();

	Super::EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled );
}

void UUGA_Misc_MoveToSight::MovementCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if(RequestID == PreviousRequestID)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s(). MovementCompleted from previous Request: %i"), *FString(__FUNCTION__), (int32)PreviousRequestID.GetID() );
		return;
	}


	ensureAlwaysMsgf( false, TEXT("%s(). Cannot rich Sight Position"), *FString(__FUNCTION__) );
	K2_EndAbility();
}

void UUGA_Misc_MoveToSight::CheckVisibilityAndDistance()
{
	ACharacter* UnitCharacter = Cast<ACharacter>( CurrentActorInfo->AvatarActor.Get() );
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( UnitCharacter ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(Character)"), *FString(__FUNCTION__) );
			K2_EndAbility();
			return;
		};
#endif
	}

	if ( !IsValid( TargetActor ) )
	{
		K2_EndAbility();
	}


	FVector Start = UnitCharacter->GetActorLocation();
	FVector End = TargetActor->GetActorLocation();

	// Если мы находимся далеко от цели, то мы уже не выполняем условия для остановки.
	if(MinimalDistance < FVector::Distance( Start, End ))
	{
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(UnitCharacter);

	if ( GetWorld()->LineTraceSingleByChannel( HitResult, Start, End, ECC_Visibility, TraceParams ) )
	{
		if ( HitResult.GetActor() == TargetActor )
		{
			AAIController* AIController = Cast<AAIController>(UnitCharacter->GetController());
			{
#if GAME_DEBUG_BUILDS
				if(!IsValid(AIController))
				{
					ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AIController)"), *FString(__FUNCTION__) );
					K2_EndAbility();
					return;
				};
#endif
			}

			K2_EndAbility();
		}
	}
}

