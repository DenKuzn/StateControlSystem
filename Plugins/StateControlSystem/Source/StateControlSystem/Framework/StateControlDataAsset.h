// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "StateControl.h"

#include "StateControlDataAsset.generated.h"

class UStateControlWidget;
class UStateControl;
class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType, Blueprintable)
struct STATECONTROLSYSTEM_API FStateControlData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStateControlWidget> StateWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStateControl> StateControlClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "StateControl" ))
	FGameplayTag StateControlTag;

};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class STATECONTROLSYSTEM_API UStateControlDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FStateControlData DefaultStateControlData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FStateControlData> AllStateControlData;

};


///////////////////////////////////////// TODO: ПОДУМАТЬ - ИМЕЕТ ЛИ СМЫСЛ ВЫНЕСТИ ЭТО В ОТДЕЛЬНЫЙ ФАЙЛ?

USTRUCT(BlueprintType, Blueprintable)
struct STATECONTROLSYSTEM_API FStateControlInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "StateControl" ))
	FGameplayTag InputTag;

};


UCLASS(BlueprintType, Blueprintable)
class STATECONTROLSYSTEM_API UStateControlInputActionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FStateControlInputAction> InputActionData;

};



USTRUCT(BlueprintType, Blueprintable)
struct STATECONTROLSYSTEM_API FStateControlUnitAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag UnitAbilityTag;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "StateControl" ))
	FGameplayTag StateControlTag;

};

/** Для одного и того же состояния могут использоваться разные абилки.
 *  Поэтому существует базовый класс SC_Order_Base в Блюпринтах, от которого наследуются все приказы для юнитов.
 *  Данная таблица нужна для связки StateControl и абилки, которую оно вызывает, когда игрок будет активировать
 *  приказ через UI.
 *	UI вызывает State. State вызывает абилку по завершении настройки игроком приказа.
 */
UCLASS(BlueprintType, Blueprintable)
class STATECONTROLSYSTEM_API UStateControlUnitAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FStateControlUnitAbility> StateControlAndAbilityConnections;


};