// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

#include "BoxStrategyWeaponSystemComponent.generated.h"

class UNiagaraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSimpleWeaponSystemDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FWeaponSystemRotationDelegate, float, NewHorizontalDirection, float, NewVerticalDirection );


class UWeaponFireControl;
class UBoxStrategyItemData_Weapon;
class UNiagaraSystem;
class USkeletalMesh;
class UAnimSequence;

/** Нет нужды в функции Stop Fire, так как оружие делает только 1 выстрел всегда и завершает работу.
 *	По завершении выстрела делегат на завершение ожидания выстрела.
 *
 */


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class BOXSTRATEGY_API UBoxStrategyWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TEST();

public:
	// Sets default values for this component's properties
	UBoxStrategyWeaponSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	FTimerHandle ReloadTimerHandle;

	FTimerHandle FireTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem", meta = (AllowPrivateAccess = "true"))
	int32 CurrentBulletsInClip = 0;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem", meta = (AllowPrivateAccess = "true"))
	bool bFiring = false;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem", meta = (AllowPrivateAccess = "true"))
	bool bNeedReload = false;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem", meta = (AllowPrivateAccess = "true"))
	bool bRealoading = false;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxStrategyItemData_Weapon> WeaponDataAsset = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMeshComponent = nullptr;

/////////////////// WE NEED ACCES TO COMPONENTS AND NEED THIS COMPONENTS TO BE LOADED IN UNIT FOR USE. OTHERWISE WE CANNOT USE EFFECTS. IT IS VERY EXPENSIVE TO CREATE THEM EVERY FIRE
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* WeaponFireEffectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* WeaponBulletEffectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* WeaponImpactEffectComponent = nullptr;


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AnimFireSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimSequence* AnimReloadSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* BulletEffect = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ImpactEffect = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* WeaponFireEffect = nullptr;

	// Async Effects Loading Data
	TSharedPtr<FStreamableHandle> BulletEffect_LoadingHandle;
	TSharedPtr<FStreamableHandle> WeaponFireEffect_LoadingHandle;
	TSharedPtr<FStreamableHandle> ImpactEffect_LoadingHandle;
	TSharedPtr<FStreamableHandle> AnimFireSequence_LoadingHandle;
	TSharedPtr<FStreamableHandle> AnimReloadSequence_LoadingHandle;



public:

	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnWeaponStartedToReloadDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnWeaponEndedReloadDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnWeaponChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnFireDelayEndedDelegate;

	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnWeaponNeedReloadDelegate;

	//TODO: Добавить функцию по вызову делегата, если компонент будет уничтожен (удален). Так же удалить все таймеры и очистить все делегаты.
	UPROPERTY(BlueprintAssignable)
	FSimpleWeaponSystemDelegate OnWeaponRemovedDelegate;


public:

	UFUNCTION(BlueprintCallable)
	void InitializeWeaponSystemComponent(USkeletalMeshComponent* NewUnitSkeletalMeshComponent,
	                                     UNiagaraComponent*		 NewWeaponFireEffectComponent,
	                                     UNiagaraComponent*		 NewWeaponBulletEffectComponent,
	                                     UNiagaraComponent*		 NewWeaponImpactEffectComponent);

	UFUNCTION(BlueprintCallable)
	void EquipNewWeapon(UBoxStrategyItemData_Weapon* NewWeaponDataAsset);

	UFUNCTION(BlueprintCallable)
	void UnequipWeapon();

	/** Function works for one fire. After fire is ended, it will start timer for next available fire.
	 * After timer ended, it will call OnFireDelayEndedDelegate.
	 * @param NewFireDirection - Will be normalized in function.
	 */
	UFUNCTION(BlueprintCallable)
	void StartFire(FVector NewFireDirection);

	UFUNCTION(BlueprintCallable)
	void StartReload();

	UFUNCTION(BlueprintCallable)
	bool IsReloading();

	UFUNCTION(BlueprintPure)
	bool IsWeaponEquipped();

	/** Max Fire Distance */
	UFUNCTION(BlueprintPure)
	float GetWeaponRange();

	UFUNCTION(BlueprintPure)
	UBoxStrategyItemData_Weapon* GetWeaponDataAsset() { return WeaponDataAsset.Get(); };

private:

	void GetWeaponStartPoint(FVector& OUT WeaponStartPoint);

	void ResetTimer(FTimerHandle& TimerHandle);

	void ReloadFinished();

	void OnFireDelayEnded();

	void AssetWasLoaded();





/*
	template <class Type>
	void AsyncLoadAsset(TSoftObjectPtr<Type> SoftObjectPtr,
	                    TSharedPtr<FStreamableHandle>& OutHandle,
	                    FStreamableManager& StreamableManager,
	                    Type** Outer, FString AssetNameForDebug = "UnknownAssetType")
	{
		OutHandle.Reset();
		OutHandle = nullptr;

		if ( IsValid( SoftObjectPtr.Get() ) )
		{
			*Outer = Cast<Type>( SoftObjectPtr.Get() );
			return;
		}

		if ( SoftObjectPtr.IsPending() )
		{
			FStreamableDelegate OnLoaded;
			OnLoaded.BindLambda( [this, &TempOutHandle = OutHandle, &Outer = *Outer]() { HandleLoadedAsset( TempOutHandle, &Outer ); } );
			OutHandle = StreamableManager.RequestAsyncLoad( SoftObjectPtr.ToSoftObjectPath(), OnLoaded );
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s(). SoftObjectPtr (%s type) empty or invalid"), *FString(__FUNCTION__), *AssetNameForDebug);
	}


	template <class Type>
	void HandleLoadedAsset(TSharedPtr<FStreamableHandle>& StreamableHandle,
	                       Type** Outer)
	{
		*Outer = Cast<Type>( StreamableHandle->GetLoadedAsset() );
		StreamableHandle.Reset();

		UE_LOG( LogTemp, Error, TEXT("%s(). Is Valid Bullet: %s"), *FString(__FUNCTION__), TEXT_TRUE_FALSE( IsValid( *Outer ) ) );
	}

*/

};
