// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableItemData_Base.h"
#include "HandWeaponData_Base.generated.h"

/**
 *
 */
UCLASS()
class BOXSTRATEGY_API UHandWeaponData_Base : public UPickableItemData_Base
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	FGameplayTag DamageType;

	//For Shotguns it will be 10, for pistols, rifles - 1.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	int32 BulletsPerCartridge = 1;

	//For Shotguns it will be some value, for pistols, rifles - 0.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	float SpreadAngle = 0.f;

	//Seconds between shots. The begger, the slower.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	float FireSpeed = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	float FireDamage = 10.f;

	//Remember units size is 10.10.20 cm. 1000 - like a whole 100 meters in real life.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Fire")
	float WeaponRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Clip")
	int32 ClipSize = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponProperties|Clip")
	float ReloadTime = 5.f;

	//TODO: Добавить звук.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSound")
	TSoftObjectPtr<USoundCue> FireSound = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSkeletalMesh")
	TSoftObjectPtr<USkeletalMesh> WeaponSkeletalMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSkeletalMesh")
	TSoftObjectPtr<UAnimSequence> AnimFireInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponSkeletalMesh")
	TSoftObjectPtr<UAnimSequence> AnimReloadInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponEffects")
	TSoftObjectPtr<UNiagaraSystem> BulletEffect = nullptr;

	//Скорей всего нужна мапа - тип поверхности = тип эффекта. Можно по тегу.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponEffects")
	TSoftObjectPtr<UNiagaraSystem> ImpactEffect = nullptr;

	//Скорей всего нужна мапа - тип поверхности = тип эффекта. Можно по тегу.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponEffects")
	TSoftObjectPtr<UNiagaraSystem> WeaponFireEffect = nullptr;


	//How fast bullet will fly. Cm per second. For now it only visual effect. Maybe when it will be fully runtime, then it can be transfered to parameters.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponEffects")
	float BulletSpeed = 500.f;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponBulletEffects", meta = (AllowPrivateAccess = "true"))
	// TSubclassOf<UWeaponFireControl> FireControl = nullptr;

};
