// Fill out your copyright notice in the Description page of Project Settings.


#include "UGA_Warrior_Attack.h"

#include "AIController.h"
#include "Global/BoxStrategyNativeGameplayTags.h"

#include "BoxStrategy/Global/BoxStrategyNativeGameplayTags.h"
#include "GeneralDebugMacroses/Framework/DebugMacroses.h"
#include "BoxStrategy/Framework/HealthSystem/BoxStrategyHealthComponent.h"
#include "BoxStrategy/Framework/WeaponSystem/BoxStrategyHandWeaponComponent.h"
#include "UnitOrderSystem/Framework/Components/UnitOrderAbilitySystemComponent.h"
#include "BoxStrategy/GameAI/AbilitySystem/AbilitySettings/Misc/UGAS_Misc_MoveToSight.h"
#include "BoxStrategy/GameAI/AbilitySystem/AbilitySettings/Characters/Warrior/UGAS_Warrior_Attack.h"

DEFINE_LOG_CATEGORY_STATIC( UGA_Warrior_Attack, Log, All );

UUGA_Warrior_Attack::UUGA_Warrior_Attack(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
	bHasBlueprintActivate = false;
	bHasBlueprintActivateFromEvent = false;
}

void UUGA_Warrior_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	// Super::ActivateAbility( Handle, ActorInfo, ActivationInfo, TriggerEventData ); // Нужна только для того, чтобы запустить абилку в БП. В нашем случае в БП ничего не запускается.

	UUnitOrderAbilitySystemComponent* AbilityComponent = GetUnitOrderAbilitySystemComponent();
	const UUGAS_Warrior_Attack* AbilitySettings = Cast<UUGAS_Warrior_Attack>( GetMyAbilitySettings() );
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( AbilityComponent ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilityComponent)"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		};

		if ( !IsValid( AbilitySettings ) || !IsValid( AbilitySettings->GetTargetActor() ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(AbilitySettings) || !IsValid(AbilitySettings->GetTargetActor())"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		}

		if ( !ActorInfo || !ActorInfo->AvatarActor.IsValid() )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !ActorInfo || !ActorInfo->AvatarActor.IsValid()"), *FString(__FUNCTION__) );
			EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
			return;
		}
#endif
	}

	TargetActor = AbilitySettings->GetTargetActor();
	bNeedChaseTarget = AbilitySettings->GetNeedChaseTarget();


	if ( !IsValid( WeaponSystemComponent ) )
	{
		WeaponSystemComponent = ActorInfo->AvatarActor->GetComponentByClass<UBoxStrategyHandWeaponComponent>();
		{
#if GAME_DEBUG_BUILDS
			if ( !IsValid( WeaponSystemComponent ) )
			{
				ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(WeaponSystemComponent)"), *FString(__FUNCTION__) );
				EndAbility( Handle, ActorInfo, ActivationInfo, true, true );
				return;
			};
#endif
		}
	}

	BindAllWeaponDelegates();

	HandleNextAttack();
}

void UUGA_Warrior_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility,
                                     bool bWasCancelled)
{
	UnbindAllWeaponDelegates();
	Super::EndAbility( Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled );
}


void UUGA_Warrior_Attack::MoveToTarget()
{
	UUnitOrderAbilitySystemComponent* UnitAbilityComponent = GetUnitOrderAbilitySystemComponent();
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( UnitAbilityComponent ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(UnitAbilityComponent)"), *FString(__FUNCTION__) );
			K2_EndAbility();
			return;
		};
#endif
	}

	// Собираем приказ для атаки, чтобы можно было повторно запустить атаку.
	{
		UUGAS_Warrior_Attack* AttackSettings = NewObject<UUGAS_Warrior_Attack>( GetOuter() );
		AttackSettings->SetAbilitySettings( TargetActor, true );
		//AttackSettings->SetAbilityTag( FBoxStrategyNativeGameplayTags::Get().BoxStrategy_Ability_Unit_Character_Warrior_Other_StandardAttack );

		UnitAbilityComponent->AddNextUnitAbilityOrderSettings( AttackSettings, true );
	}

	// Собираем приказ для движения, пока не будет виден противник, чтобы можно было в него стрелять напрямую.
	{
		UUGAS_Misc_MoveToSight* MoveToSightSettings = NewObject<UUGAS_Misc_MoveToSight>( GetOuter() );
		MoveToSightSettings->SetTargetActor( TargetActor );
		//MoveToSightSettings->SetAbilityTag( FBoxStrategyNativeGameplayTags::Get().BoxStrategy_Ability_Unit_Character_Misc_MoveToSight );
		MoveToSightSettings->SetMinimalDistance( WeaponSystemComponent->GetWeaponRange() - 50.f );

		UnitAbilityComponent->AddNextUnitAbilityOrderSettings( MoveToSightSettings );
	}

	UnitAbilityComponent->ExecuteNextUnitOrder();
	return;
}

void UUGA_Warrior_Attack::HandleNextAttack()
{
	/** Правка на будущее, если будет вписан компонент поворота к цели юнита.
	 *	Если еще не запущен трекинг на цель, то нужно подписываться перед выстрелом на делегат, который будет вызываться, когда юнит повернется к цели.
	 *	Дождаться, когда повернется и только потом стрелять.
	 *	При этом трекинг должен быть до тех пор, пока выполняется абилка. */


	check( TargetActor );
	check( WeaponSystemComponent );
	check( CurrentActorInfo );
	check( CurrentActorInfo->AvatarActor.Get() );

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();

	if ( !WeaponSystemComponent->IsWeaponEquipped() )
	{
		K2_EndAbility();
		return;
	}

	UBoxStrategyHealthComponent* TargetHealthComponent = TargetActor->GetComponentByClass<UBoxStrategyHealthComponent>();
	{
#if GAME_DEBUG_BUILDS
		if ( !IsValid( TargetHealthComponent ) )
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(TargetHealthComponent)"), *FString(__FUNCTION__) );
			K2_EndAbility();
			return;
		};
#endif
	}

	if ( TargetHealthComponent->IsDeadOrDying() )
	{
		K2_EndAbility();
		return;
	}


	FVector VectorToTarget = TargetActor->GetActorLocation() - AvatarActor->GetActorLocation();

	// Мы дальше, чем нужно?
	if ( WeaponSystemComponent->GetWeaponRange() < VectorToTarget.Length() )
	{
		if(bNeedChaseTarget)
		{
			MoveToTarget();
		}
		else
		{
			// Мы завершаем абилку, так как не можем достигнуть цели, а двигаться нам запрещено.
			K2_EndAbility();
		}

		return;
	}


	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor( AvatarActor );

	GetWorld()->LineTraceSingleByChannel( HitResult, AvatarActor->GetActorLocation(), TargetActor->GetActorLocation(), ECollisionChannel::ECC_Visibility, TraceParams );


	// Проверяем, видим ли противника.
	if ( HitResult.GetActor() != TargetActor )
	{
		if(bNeedChaseTarget)
		{
			MoveToTarget();
		}
		else
		{
			// Мы завершаем абилку, так как не можем достигнуть цели, а двигаться нам запрещено.
			K2_EndAbility();
		}

		return;
	}

	// Если видим, если дистанция норм, то запускаем выстрел.
	FRotator RotatorToTarget = FRotationMatrix::MakeFromX(VectorToTarget).Rotator();

	AvatarActor->SetActorRotation(  FRotator(0, RotatorToTarget.Yaw, 0.f ) );

	WeaponSystemComponent->StartFire( VectorToTarget );
}

void UUGA_Warrior_Attack::StartReloadWeapon()
{
	check( WeaponSystemComponent );
	WeaponSystemComponent->StartReload();
}

void UUGA_Warrior_Attack::BindAllWeaponDelegates()
{
	if( !IsValid(WeaponSystemComponent) )
	{
		// Если компонент оружия не валиден, то и делать ничего не нужно.
		return;
	}

	WeaponSystemComponent->OnFireDelayEndedDelegate.AddDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
	WeaponSystemComponent->OnWeaponRemovedDelegate.AddDynamic( this, &UUGA_Warrior_Attack::K2_EndAbility );
	WeaponSystemComponent->OnWeaponChangedDelegate.AddDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
	WeaponSystemComponent->OnWeaponNeedReloadDelegate.AddDynamic( this, &UUGA_Warrior_Attack::StartReloadWeapon );
	WeaponSystemComponent->OnWeaponEndedReloadDelegate.AddDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
}

void UUGA_Warrior_Attack::UnbindAllWeaponDelegates()
{
	if( !IsValid(WeaponSystemComponent) )
	{
		// Если компонент оружия не валиден, то и делать ничего не нужно.
		return;
	}

	WeaponSystemComponent->OnFireDelayEndedDelegate.RemoveDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
	WeaponSystemComponent->OnWeaponRemovedDelegate.RemoveDynamic( this, &UUGA_Warrior_Attack::K2_EndAbility );
	WeaponSystemComponent->OnWeaponChangedDelegate.RemoveDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
	WeaponSystemComponent->OnWeaponNeedReloadDelegate.RemoveDynamic( this, &UUGA_Warrior_Attack::StartReloadWeapon );
	WeaponSystemComponent->OnWeaponEndedReloadDelegate.RemoveDynamic( this, &UUGA_Warrior_Attack::HandleNextAttack );
}
