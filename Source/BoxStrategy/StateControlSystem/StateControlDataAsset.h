// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"

#include "StateControl.h"

#include "StateControlDataAsset.generated.h"

class UStateControlWidget;
class UStateControl;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BOXSTRATEGY_API UStateControlDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStateControlWidget> StateWidgetClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStateControl> StateControlClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "StateControl" ))
	FGameplayTag StateControlTag;

};


///////////////////////////////////////// TODO: ПОДУМАТЬ - ИМЕЕТ ЛИ СМЫСЛ ВЫНЕСТИ ЭТО В ОТДЕЛЬНЫЙ ФАЙЛ?

USTRUCT()
struct BOXSTRATEGY_API FStateControlInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "StateControl" ))
	FGameplayTag InputTag;

};


UCLASS(BlueprintType, Blueprintable)
class BOXSTRATEGY_API UStateControlInputActionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<FStateControlInputAction> InputActionData;

};
