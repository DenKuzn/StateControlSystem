// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "MyExtensionTypes.generated.h"

/**
 *
 */
USTRUCT()
struct MYEXTENSION_API FGameplayTagPropertyMapping
{
	GENERATED_BODY()

	FGameplayTagPropertyMapping() {}
	FGameplayTagPropertyMapping(const FGameplayTagPropertyMapping& Other)
	{
		// Don't copy handle
		TagToMap = Other.TagToMap;
		PropertyToEdit = Other.PropertyToEdit;
		PropertyName = Other.PropertyName;
		PropertyGuid = Other.PropertyGuid;
	}

	/** Gameplay tag being counted. */
	UPROPERTY(EditAnywhere, Category = GameplayTagBlueprintProperty)
	FGameplayTag TagToMap;

	/** Property to update with the gameplay tag count. */
	UPROPERTY(VisibleAnywhere, Category = GameplayTagBlueprintProperty)
	TFieldPath<FBoolProperty> PropertyToEdit;

	UPROPERTY(VisibleAnywhere, Category = GameplayTagBlueprintProperty)
	FName PropertyName;

	UPROPERTY(VisibleAnywhere, Category = GameplayTagBlueprintProperty)
	FGuid PropertyGuid;

	FDelegateHandle DelegateHandle;

};

USTRUCT()
struct MYEXTENSION_API FGameplayTagPropertyMap
{
	GENERATED_BODY()

	void GameplayTagEventCallback(const FGameplayTag Tag, int32 NewCount);

	// Просто ссылка на существующий БП.
	void Initialize(UObject* NewOwner);

	bool IsPropertyTypeValid(const FProperty* Property) const;

	void ResetAllValuesToZero();

#if WITH_EDITOR
	/** This can optionally be called in the owner's IsDataValid() for data validation. */
	EDataValidationResult IsDataValid(UObject* ContainingAsset, TArray<FText>& ValidationErrors);
#endif // #if WITH_EDITOR


private:

	UObject* Owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "GameplayInputTags", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTagPropertyMapping> InputPropertyMappings;
};



///////////////////// ВСЕ, ЧТО НИЖЕ - ОСТАВИТЬ. ВСЕ, ЧТО ВЫШЕ - ПЕРЕНЕСТИ В ПЛАГИН ////////////////////


