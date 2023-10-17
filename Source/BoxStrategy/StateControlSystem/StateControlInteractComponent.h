// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "StateControlInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FSelectStateDelegate );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FComponentWasChangedDelegate, UStateControlInteractComponent*, StateControlInteractComponent );

/***
 * Component that must be placed in actors that can be clicked and used for interact. For example units, buildings.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class BOXSTRATEGY_API UStateControlInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateControlInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(BlueprintAssignable)
	FSelectStateDelegate ObjectWasSeleceted;

	UPROPERTY(BlueprintAssignable)
	FSelectStateDelegate ObjectWasDeseleceted;

	UPROPERTY(BlueprintAssignable)
	FComponentWasChangedDelegate StateTagWasChanged;

	UPROPERTY(BlueprintAssignable)
	FComponentWasChangedDelegate InteractComponentStateWasChanged;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "StateControl", AllowPrivateAccess = "true" ))
	FGameplayTag StateControlTag;

	bool bSelected = false;

	bool bInteractionEnabled = true;

public:

	UFUNCTION(BlueprintPure)
	void GetStateControlTag(FGameplayTag& OutStateControlTag);

	UFUNCTION(BlueprintCallable)
	void ChangeStateControlTag(FGameplayTag& NewStateControlTag);

	UFUNCTION(BlueprintCallable)
	bool ActivateObjectSelection();

	UFUNCTION(BlueprintCallable)
	void DeactivateObjectSelection();

	UFUNCTION(BlueprintCallable)
	void SetInteraction(bool bIsInteractionEnabled = true);

	UFUNCTION(BlueprintPure)
	bool IsComponentSelected() const { return bSelected; };

	UFUNCTION(BlueprintPure)
	bool IsInteractionEnabled() const { return bInteractionEnabled; };


};
