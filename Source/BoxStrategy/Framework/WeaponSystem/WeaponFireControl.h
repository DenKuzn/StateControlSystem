// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"

//#include "WeaponFireControl.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FWeaponFireControlRotationDelegate, float, NewHorizontalDirection, float, NewVerticalDirection );


// >>>>>>>>>>>>>>>>>>>>>>>> НЕ ИСПОЛЬЗУЕТСЯ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //


/** Класс, который будет отвечать за то, как оружие будет стрелять.
 * Очередью, одиночным выстрелом, и так далее.
 *
 */

/*
UCLASS()
class BOXSTRATEGY_API UWeaponFireControl : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY(BlueprintReadWrite, Category = "WeaponSystem")
	bool bFiring = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSystem")
	bool bNeedTick = false;

	UPROPERTY(BlueprintReadOnly, Category = "WeaponSystem")
	UBoxStrategyItemData_Weapon* WeaponDataAsset = nullptr;

public:

	UPROPERTY(BlueprintAssignable)
	FWeaponFireControlRotationDelegate OnWeaponDirectionChanged;

public:

	// Use standard Timers better.
	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponSystem")
	void FireControlTick(float DeltaSeconds);

	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponSystem")
	void StopFire();

	UFUNCTION(BlueprintImplementableEvent, Category = "WeaponSystem")
	void StartFire();

	UFUNCTION(BlueprintPure)
	bool IsNeedTick() { return bNeedTick; };

	UFUNCTION(BlueprintPure)
	bool IsFiring() { return bFiring; };

};
*/