// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxStrategyHandWeaponComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"

#include "BoxStrategy/Framework/HealthSystem/BoxStrategyHealthComponent.h"
#include "BoxStrategy/System/BoxStrategyAssetManager.h"
#include "Framework/ProductionSystem/ProductDataAssets/PickableItems/HandWeaponData_Base.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"


void UBoxStrategyHandWeaponComponent::TEST()
{
	UE_LOG(LogTemp, Error, TEXT("%s(). Is Valid Bullet: %s"), *FString(__FUNCTION__), TEXT_TRUE_FALSE( IsValid( BulletEffect ) ));
}

// Sets default values for this component's properties
UBoxStrategyHandWeaponComponent::UBoxStrategyHandWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBoxStrategyHandWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

// Called every frame
void UBoxStrategyHandWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	// ...
}

void UBoxStrategyHandWeaponComponent::InitializeHandWeaponComponent(USkeletalMeshComponent* NewUnitSkeletalMeshComponent,
                                                                        UNiagaraComponent* NewWeaponFireEffectComponent,
                                                                        UNiagaraComponent* NewWeaponBulletEffectComponent,
                                                                        UNiagaraComponent* NewWeaponImpactEffectComponent)
{
	WeaponSkeletalMeshComponent = NewUnitSkeletalMeshComponent;
	WeaponFireEffectComponent = NewWeaponFireEffectComponent;
	WeaponBulletEffectComponent = NewWeaponBulletEffectComponent;
	WeaponImpactEffectComponent = NewWeaponImpactEffectComponent;
}

void UBoxStrategyHandWeaponComponent::EquipNewWeapon(UHandWeaponData_Base* NewWeaponDataAsset)
{
	FUNCTION_START_LOG();

	// Checking for valid pointer.
	{
#if GAME_DEBUG_BUILDS
		GAME_DEBUG_CHECK_UNREALPOINTER_WITH_RETURN( NewWeaponDataAsset, );

		if(bRealoading)
		{
			ensureAlwaysMsgf( false, TEXT("%s(). Cannot change weapon while weapon is reloading"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}



	WeaponDataAsset = NewWeaponDataAsset;

	ResetTimer( ReloadTimerHandle );

	CurrentBulletsInClip = WeaponDataAsset->ClipSize;

	//Loading Weapon Skeletal Mesh
	{
		USkeletalMesh* NewWeaponSkeletalMesh = nullptr;

		if ( IsValid( WeaponDataAsset->WeaponSkeletalMesh.Get() ) )
		{
			NewWeaponSkeletalMesh = WeaponDataAsset->WeaponSkeletalMesh.Get();
		}
		else if ( WeaponDataAsset->WeaponSkeletalMesh.IsPending() )
		{
			NewWeaponSkeletalMesh = WeaponDataAsset->WeaponSkeletalMesh.LoadSynchronous();
		}

		if ( IsValid( WeaponSkeletalMeshComponent ) && IsValid( NewWeaponSkeletalMesh ) )
		{
			WeaponSkeletalMeshComponent->SetSkeletalMesh( NewWeaponSkeletalMesh );
		}
		else
		{
			UE_LOG( LogTemp, Error, TEXT("%s(). WeaponSkeletalMeshComponent is: %s. NewWeaponSkeletalMesh is: %s."), *FString(__FUNCTION__),
			        TEXT_TRUE_FALSE( IsValid( WeaponSkeletalMeshComponent ) ),
			        TEXT_TRUE_FALSE( IsValid( NewWeaponSkeletalMesh ) ) );
		}
	}

	OnWeaponChangedDelegate.Broadcast();

	UBoxStrategyAssetManager& BoxStrategyAssetmanager = UBoxStrategyAssetManager::Get();

	BoxStrategyAssetmanager.AsyncLoadAsset( WeaponDataAsset->BulletEffect,		&BulletEffect,			FStreamableDelegate(), "BulletEffect"  );
	BoxStrategyAssetmanager.AsyncLoadAsset( WeaponDataAsset->WeaponFireEffect,	&WeaponFireEffect,		FStreamableDelegate(), "WeaponFireEffect"  );
	BoxStrategyAssetmanager.AsyncLoadAsset( WeaponDataAsset->ImpactEffect,		&ImpactEffect,			FStreamableDelegate(), "ImpactEffect"  );

	BoxStrategyAssetmanager.AsyncLoadAsset( WeaponDataAsset->AnimFireInstance,	&AnimFireSequence,		FStreamableDelegate(), "AnimFireSequence"  );
	BoxStrategyAssetmanager.AsyncLoadAsset( WeaponDataAsset->AnimReloadInstance,	&AnimReloadSequence,	FStreamableDelegate(), "AnimReloadSequence"  );

	FUNCTION_END_LOG();
}

void UBoxStrategyHandWeaponComponent::UnequipWeapon()
{
	{
#if GAME_DEBUG_BUILDS
		if(!IsWeaponEquipped())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsWeaponEquipped()"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	ResetTimer( ReloadTimerHandle );

	WeaponDataAsset = nullptr;
	OnWeaponChangedDelegate.Broadcast();
}

void UBoxStrategyHandWeaponComponent::StartFire(FVector NewFireDirection)
{
	// Выстрел происходит в момент запуска функции. Если таймер активен - значит, выстрел уже был осуществлен недавно, и оружие не готово выпустить новую пулю.
	if(GetWorld()->GetTimerManager().IsTimerActive( FireTimerHandle ))
	{
		UE_LOG(LogTemp, Error, TEXT("%s(). Fire timer is active. Weapon is not ready to fire."), *FString(__FUNCTION__) );
		return;
	}

	if ( bRealoading )
	{
		return;
	}

	// Need to reload.
	if(CurrentBulletsInClip <= 0)
	{
		OnWeaponNeedReloadDelegate.Broadcast();
		return;
	}

	//UE_LOG(LogTemp, Error, TEXT("%s(). Create New Fire."), *FString(__FUNCTION__) );

	GetWorld()->GetTimerManager().SetTimer( FireTimerHandle, this, &UBoxStrategyHandWeaponComponent::OnFireDelayEnded, WeaponDataAsset->FireSpeed, false );
	CurrentBulletsInClip--;

	NewFireDirection.Normalize();
	FVector WeaponStartPoint;
	GetWeaponStartPoint( WeaponStartPoint );
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel( HitResult, WeaponStartPoint, WeaponStartPoint + NewFireDirection * WeaponDataAsset->WeaponRange, ECollisionChannel::ECC_Visibility );


	// Выпустить эффект пули из дула в сторону точки попадания.
	if ( IsValid( WeaponBulletEffectComponent ) && IsValid( BulletEffect ) )
	{
		float BulletLifetime = ( HitResult.bBlockingHit ? HitResult.Distance : WeaponDataAsset->WeaponRange ) / WeaponDataAsset->BulletSpeed;
		WeaponBulletEffectComponent->SetAsset( BulletEffect );
		WeaponBulletEffectComponent->SetFloatParameter( "BulletLifetime", BulletLifetime );
		WeaponBulletEffectComponent->SetFloatParameter( "BulletSpeed", WeaponDataAsset->BulletSpeed );
		WeaponBulletEffectComponent->SetVectorParameter( "BulletDirection", NewFireDirection );
		WeaponBulletEffectComponent->Activate(true);
	}

	//
	if ( IsValid( WeaponFireEffectComponent ) && IsValid( WeaponFireEffect ) )
	{
		WeaponFireEffectComponent->SetAsset( WeaponFireEffect );
		WeaponFireEffectComponent->Activate(true);
	}

	// Если есть скелетал меш и анимация выстрела - включить ее.
	if ( IsValid( WeaponSkeletalMeshComponent ) && WeaponSkeletalMeshComponent->GetSkeletalMeshAsset() && IsValid( AnimFireSequence ) )
	{
		WeaponSkeletalMeshComponent->PlayAnimation( AnimFireSequence, false );
	}

	// Если пуля пролетела мимо и ни на что не наткнулась, то дальнейший код бессмысленный.
	if(!HitResult.bBlockingHit || !IsValid(HitResult.GetActor()))
	{
		return;
	}

	if(UBoxStrategyHealthComponent* HealthComponent = HitResult.GetActor()->GetComponentByClass<UBoxStrategyHealthComponent>())
	{
		if(!HealthComponent->IsDeadOrDying())
		{
			HealthComponent->AddDamage(WeaponDataAsset->FireDamage, WeaponDataAsset->DamageType);
		}
	}

	// TODO: Добавить Импакт эффект.
	// Так как есть импакт, то указать таймер с запуском импакта в точке попадания.
	// Если импакта нет, то эффекта нет.
	// Возможно, нужно создавать отдельный класс с эффектом для этого импакта, чтобы не следить за таймером и прочей фигней.

}

void UBoxStrategyHandWeaponComponent::StartReload()
{
	{
#if GAME_DEBUG_BUILDS
		if(!IsWeaponEquipped())
		{
			ensureAlwaysMsgf( false, TEXT("%s(). !IsWeaponEquipped()"), *FString(__FUNCTION__) );
			return;
		}
#endif
	}

	if(GetWorld()->GetTimerManager().IsTimerActive( ReloadTimerHandle ))
	{
		return;
	}

	ResetTimer( FireTimerHandle );

	bRealoading = true;

	GetWorld()->GetTimerManager().SetTimer( ReloadTimerHandle, this, &UBoxStrategyHandWeaponComponent::ReloadFinished, WeaponDataAsset->ReloadTime, false );

	if(IsValid(WeaponSkeletalMeshComponent) && IsValid(WeaponSkeletalMeshComponent->GetSkeletalMeshAsset()) && IsValid(AnimReloadSequence))
	{
		WeaponSkeletalMeshComponent->PlayAnimation( AnimReloadSequence, false );
	}

	OnWeaponStartedToReloadDelegate.Broadcast();
}

bool UBoxStrategyHandWeaponComponent::IsReloading()
{
	return bRealoading;
}

bool UBoxStrategyHandWeaponComponent::IsWeaponEquipped()
{
	if(IsValid(WeaponDataAsset))
	{
		return true;
	}

	return false;
}

float UBoxStrategyHandWeaponComponent::GetWeaponRange()
{
	if(!IsWeaponEquipped())
	{
		return 0;
	}

	return WeaponDataAsset->WeaponRange;
}

void UBoxStrategyHandWeaponComponent::ReloadFinished()
{
	CurrentBulletsInClip = WeaponDataAsset->ClipSize;
	bRealoading = false;

	OnWeaponEndedReloadDelegate.Broadcast();
}

void UBoxStrategyHandWeaponComponent::OnFireDelayEnded()
{
	// We need to reset timer before broadcast, because outer can start new fire in this frame, and timer will be available still.
	ResetTimer( FireTimerHandle );
	OnFireDelayEndedDelegate.Broadcast();
}

void UBoxStrategyHandWeaponComponent::AssetWasLoaded()
{
	UE_LOG(LogTemp, Error, TEXT("%s(). Asset was loaded. Success: %s"), *FString(__FUNCTION__), TEXT_TRUE_FALSE( IsValid(BulletEffect) ) );
}

void UBoxStrategyHandWeaponComponent::GetWeaponStartPoint(FVector& WeaponStartPoint)
{
	WeaponStartPoint = FVector::Zero();

	if(IsValid( WeaponSkeletalMeshComponent ) && WeaponSkeletalMeshComponent->GetSkeletalMeshAsset())
	{
		WeaponStartPoint = WeaponSkeletalMeshComponent->GetSocketLocation( "WeaponStartPoint" );
	}

	if ( WeaponStartPoint.IsZero() )
	{
		WeaponStartPoint = GetOwner()->GetActorLocation();
	}
}

void UBoxStrategyHandWeaponComponent::ResetTimer(FTimerHandle& TimerHandle)
{
	if(GetWorld()->GetTimerManager().IsTimerActive( TimerHandle ))
	{
		GetWorld()->GetTimerManager().ClearTimer( TimerHandle );
	}
}



/*
	if(WeaponDataAsset->BulletEffect.IsValid() && IsValid( WeaponDataAsset->BulletEffect.Get() ))
	{
		BulletEffect = WeaponDataAsset->BulletEffect.Get();
	}
	else if(WeaponDataAsset->BulletEffect.IsPending())
	{

		BulletEffecLoadingHandle = StreamableManager.RequestAsyncLoad( WeaponDataAsset->BulletEffect.ToSoftObjectPath(), FStreamableDelegate::CreateUObject( this, &UWeaponSystemComponent::OnBulletEffectLoaded ) );
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s(). Bullet Effect is not valid and not pending"), *FString(__FUNCTION__));
	}
*/
