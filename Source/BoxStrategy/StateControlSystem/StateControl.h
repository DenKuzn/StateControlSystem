// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "MyExtension/Framework/MyExtensionTypes.h"

#include "StateControl.generated.h"

class UStateControlInteractComponent;
class UStateControlSystemComponent;
class UStateControlWidget;
class UStateControlInputActionDataAsset;

class UEnhancedInputComponent;

/**

Класс управления игроком интерактивными объектами в игре.
Класс берет управление над игрой при активации и дальше принимает решение, что делать, когда игрок делает те или иные действия.

------------------------------------------------------->>>>>>>> ENGSLISH <<<<<<<<<<-------------------------------------------------------

The class is designed to work in BP.
The class for controlling the player's interaction with game objects.
The class takes control of the game upon activation and then decides what to do when the player performs certain actions.

StateControlInputTriggered for Trigger hot keys with Input GameplayTag.

 */
UCLASS(Blueprintable, BlueprintType, Category = "StateControl")
class BOXSTRATEGY_API UStateControl : public UObject
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
	//void TEST_FUNCTION(bool Newbool, FGameplayTag SearchedTag);

public:

	virtual UWorld* GetWorld() const override;
	bool StateControlHasInputs();

private:

	UPROPERTY(BlueprintReadWrite, Category = "StateControl", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlWidget> StateWidgetInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "StateControl", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStateControlWidget> StateWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StateControl", meta = (AllowPrivateAccess = "true"))
	bool bConsumeActions = true;

	UPROPERTY(EditDefaultsOnly, Category = "StateControl|InputData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateControlInputActionDataAsset> InputData;

	UPROPERTY(EditDefaultsOnly, Category = "StateControl|InputData", meta = (AllowPrivateAccess = "true"))
	FGameplayTagPropertyMap InputTagPropertyMap;

public:
	void InitializeStateControl( TSubclassOf<UStateControlWidget> StateWidget = nullptr );

	UFUNCTION(BlueprintCallable)
	void ActivateHotKeys();

	UFUNCTION(BlueprintCallable)
	void DeactivateHotKeys();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "StateControlActivation", DisplayName = "ActivateState", meta=(ScriptName = "ActivateState"))
	void K2_ActivateState();
	virtual void ActivateState();

	UFUNCTION(BlueprintImplementableEvent, Category = "StateControlActivation", DisplayName = "DeactivateState", meta=(ScriptName = "DeactivateState"))
	void K2_DeactivateState();
	virtual void DeactivateState();

/*
	UFUNCTION(BlueprintImplementableEvent, Category = "StateControlAction", DisplayName = "ActivateMainAction", meta=(ScriptName = "ActivateMainAction"))
	void K2_ActivateMainAction();
	virtual void ActivateMainAction();

	UFUNCTION(BlueprintImplementableEvent, Category = "StateControlAction", DisplayName = "ActivateBackAction", meta=(ScriptName = "ActivateBackAction"))
	void K2_ActivateBackAction();
	virtual void ActivateBackAction();

	UFUNCTION(BlueprintImplementableEvent, Category = "StateControlAction", DisplayName = "ActivateSecondAction", meta=(ScriptName = "ActivateSecondAction"))
	void K2_ActivateSecondAction();
	virtual void ActivateSecondAction();
*/

	UFUNCTION(BlueprintImplementableEvent, Category = "StateControlInput")
	void StateControlInputTriggered(FGameplayTag InputTag, bool bPressed);

	UFUNCTION(BlueprintPure)
	bool IsConsumeActions();

	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponentWithMyTagUnderMouse(ECollisionChannel TraceChannel = ECC_Camera, bool bTraceComplex = true);

	UFUNCTION(BlueprintPure)
	UStateControlInteractComponent* GetInteractComponentUnderMouse(ECollisionChannel TraceChannel = ECC_Camera, bool bTraceComplex = true);

	UFUNCTION(BlueprintPure)
	bool IsMyTag(FGameplayTag StateControlTag);

	// This function says that this state in the active states array in System Component.
	// But it is not knows if this state in last in or some state overed it.
	UFUNCTION(BlueprintPure)
	bool IsActiveStateControl();

	UFUNCTION(BlueprintCallable) // доступно для теста.
	void NewInputActionState(FGameplayTag StateControlInputTag, bool NewState);


public:

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
	//virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const;
#endif




private:

	UFUNCTION(BlueprintPure)
	UStateControlSystemComponent* GetStateControlSystemComponent();

	UFUNCTION(BlueprintCallable, Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsComponentInteractionEnabled(UStateControlInteractComponent* InteractComponent);

	bool IsInstantiated() const;

	void Input_InputTagPressed(FGameplayTag InputTag);
	void Input_InputTagReleased(FGameplayTag InputTag);

	UEnhancedInputComponent* GetInputComponent();



};
