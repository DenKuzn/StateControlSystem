// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"

#include "GeneralDebugMacroses/Framework/DebugMacroses.h"

#include "BoxStrategyHandWeaponComponent.generated.h"

class UNiagaraComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSimpleWeaponSystemDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FWeaponSystemRotationDelegate, float, NewHorizontalDirection, float, NewVerticalDirection );

class UHandWeaponData_Base;
class UWeaponFireControl;
class UNiagaraSystem;
class USkeletalMesh;
class UAnimSequence;

/** This Component represents all hand weapons that can be used by settlers and other unit types with hands.
 *
 * We don't need Stop Fire function because the weapon make only one shot and always stops fire.
 *	По завершении выстрела делегат на завершение ожидания выстрела (I don't get it. Forgot meaning =) ).
 *
 */


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class BOXSTRATEGY_API UBoxStrategyHandWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void TEST();

public:
	// Sets default values for this component's properties
	UBoxStrategyHandWeaponComponent();

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
	TObjectPtr<UHandWeaponData_Base> WeaponDataAsset = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent = nullptr;

/////////////////// WE NEED ACCES TO COMPONENTS AND NEED THIS COMPONENTS TO BE LOADED IN UNIT FOR USE. OTHERWISE WE CANNOT USE EFFECTS. IT IS VERY EXPENSIVE TO CREATE THEM EVERY FIRE
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> WeaponFireEffectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> WeaponBulletEffectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> WeaponImpactEffectComponent = nullptr;


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
	void InitializeHandWeaponComponent(USkeletalMeshComponent* NewUnitSkeletalMeshComponent,
	                                     UNiagaraComponent*		 NewWeaponFireEffectComponent,
	                                     UNiagaraComponent*		 NewWeaponBulletEffectComponent,
	                                     UNiagaraComponent*		 NewWeaponImpactEffectComponent);

	UFUNCTION(BlueprintCallable)
	void EquipNewWeapon(UHandWeaponData_Base* NewWeaponDataAsset);

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
	UHandWeaponData_Base* GetWeaponDataAsset() { return WeaponDataAsset.Get(); };

private:

	void GetWeaponStartPoint(FVector& OUT WeaponStartPoint);

	void ResetTimer(FTimerHandle& TimerHandle);

	void ReloadFinished();

	void OnFireDelayEnded();

	void AssetWasLoaded();

};
