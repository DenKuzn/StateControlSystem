// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateControlSystemComponent.generated.h"


struct FGameplayTag;
class UStateControlDataAsset;
class UStateControl;




/*
 *	Actions in States.
	The principle of activating Actions in States will follow the LIFO (Last In, First Out) order. That is, the one who is last in the activation array will be activated.
	States remove themselves from the array when they are canceled or when they pass control to others, so there's no need to use delegates.
	It's sufficient to monitor the position of states in the array and shift the entire array when they are removed.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class BOXSTRATEGY_API UStateControlSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateControlSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	//Cache Reference on DA with Base State Control separatly.
	UPROPERTY(EditDefaultsOnly, Category = "StateControlSystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlDataAsset> StateControlData = nullptr;

	// Stores all created state controls mapped with Tag.
	UPROPERTY()
	TMap<FGameplayTag, UStateControl*> StateControlsStorage;

	// All current active states..
	UPROPERTY()
	TArray<UStateControl*> CurrentActiveStates;

public:

	/** Be sure that you will activate state manually when you turn off AutoActivate*/
	UFUNCTION(BlueprintCallable)
	UStateControl* SetNewActiveStateByTag(FGameplayTag NewStateTag, bool NeedAutoActivate = true);

	UFUNCTION(BlueprintCallable)
	void DeactivateState(UStateControl* SearchedStateControl);

	// /** Be careful. States have own input system and dont need to use Main Action */
	// UFUNCTION(BlueprintCallable)
	// void ActivateMainAction();
	//
	// UFUNCTION(BlueprintCallable)
	// void ActivateBackAction();
	//
	// UFUNCTION(BlueprintCallable)
	// void ActivateSecondAction();

	UFUNCTION(BlueprintCallable)
	UStateControl* DeactivateStateByTag(FGameplayTag SearchedStateTag);

	UFUNCTION(BlueprintPure)
	bool IsTagForStateControl(FGameplayTag SearchedStateTag, UStateControl* SearchedStateControl);


	UFUNCTION(BlueprintPure)
	bool IsStateControlActive(UStateControl* StateControl);

	UFUNCTION(BlueprintPure, meta = (ExpandBoolAsExecs))
	bool IslocalyControlled();


	// ONLY FOR TEST
	UFUNCTION(BlueprintPure, Category = "Test State Control System")
	const TMap<FGameplayTag, UStateControl*>& GetStateControlsStorage() { return StateControlsStorage; };
	// ONLY FOR TEST
	UFUNCTION(BlueprintPure, Category = "Test State Control System")
	const TArray<UStateControl*>& GetCurrentActiveStates() { return CurrentActiveStates; };

};
