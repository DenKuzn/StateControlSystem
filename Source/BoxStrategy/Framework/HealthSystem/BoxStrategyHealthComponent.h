// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/GameFrameworkComponent.h"

#include "BoxStrategyHealthComponent.generated.h"

/**
 * EBoxStrategyDeathState
 *
 *	Defines current state of death. All states after NotDead = Dead.
 */
UENUM(BlueprintType)
enum class EBoxStrategyDeathState : uint8
{
	NotDead = 0,

	DeathStarted,

	DeathFinished
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSimpleHealthSystemDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FDamageHealthSystemDelegate, float, NewDamage, FGameplayTag, DamageType );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class BOXSTRATEGY_API UBoxStrategyHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoxStrategyHealthComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(BlueprintReadOnly, Category = "HealthSystem", meta = (AllowPrivateAccess = "true"))
	EBoxStrategyDeathState DeathState = EBoxStrategyDeathState::NotDead;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthSystem", meta = (AllowPrivateAccess = "true"))
	float BaseHealth = 100.f;

	UPROPERTY(BlueprintReadOnly, Category = "HealthSystem", meta = (AllowPrivateAccess = "true"))
	float CurrentDamage = 0.f;

	/** When 0 = Immune to this type of damage. When 1 = Normal damage. When > 1 = More damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthSystem", meta = (AllowPrivateAccess = "true"))
	TMap <FGameplayTag, float> DamageMultiplier;

public:

	UPROPERTY(BlueprintAssignable, Category = "HealthSystem")
	FSimpleHealthSystemDelegate OnDeadStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "HealthSystem")
	FSimpleHealthSystemDelegate OnDeadFinishedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "HealthSystem")
	FDamageHealthSystemDelegate OnDamageDelegate;

public:


	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "BoxStrategy|Health")
	static UBoxStrategyHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UBoxStrategyHealthComponent>() : nullptr); }


	UFUNCTION(BlueprintCallable)
	void AddDamage(float NewDamage, FGameplayTag DamageType);

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "BoxStrategy|Health")
	float GetCurrentHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "BoxStrategy|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "BoxStrategy|Health")
	float GetCurrentHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "BoxStrategy|Health")
	EBoxStrategyDeathState GetDeathState() const; // { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "BoxStrategy|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const; //{ return (DeathState > EBoxStrategyDeathState::NotDead); }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

};
