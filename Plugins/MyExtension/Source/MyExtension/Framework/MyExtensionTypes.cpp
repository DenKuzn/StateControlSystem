// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.


#include "MyExtensionTypes.h"

void FGameplayTagPropertyMap::GameplayTagEventCallback(const FGameplayTag Tag, int32 NewCount)
{

	if(!IsValid(Owner))
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !IsValid(Owner)"), *FString(__FUNCTION__) );
		return;
	};

	if(!Tag.IsValid())
	{
		ensureAlwaysMsgf( false, TEXT("%s(). !Tag.IsValid()"), *FString(__FUNCTION__) );
		return;
	};

	FGameplayTagPropertyMapping* Mapping = InputPropertyMappings.FindByPredicate([Tag](const FGameplayTagPropertyMapping& Test)
	{
		return (Tag == Test.TagToMap);
	});

	if (Mapping && Mapping->PropertyToEdit.Get())
	{

		if (const FBoolProperty* BoolProperty = CastField<const FBoolProperty>(Mapping->PropertyToEdit.Get()))
		{
			BoolProperty->SetPropertyValue_InContainer(Owner, NewCount > 0);
		}
		else
		{
			ensureAlwaysMsgf( false, TEXT("%s(). CastField to FBoolProperty Failed"), *FString(__FUNCTION__) );
			return;
		}

		/*
		else if (const FIntProperty* IntProperty = CastField<const FIntProperty>(Mapping->PropertyToEdit.Get()))
		{
			IntProperty->SetPropertyValue_InContainer(Owner, NewCount);
		}
		else if (const FFloatProperty* FloatProperty = CastField<const FFloatProperty>(Mapping->PropertyToEdit.Get()))
		{
			FloatProperty->SetPropertyValue_InContainer(Owner, (float)NewCount);
		}
		*/

	}
}

void FGameplayTagPropertyMap::Initialize(UObject* NewOwner)
{
	Owner = NewOwner;
	if(!IsValid( Owner ))
	{
		ensureAlwaysMsgf( false, TEXT("%s(). Owner Invalid"), *FString(__FUNCTION__) );
		return;
	}

	for (int32 MappingIndex = (InputPropertyMappings.Num() - 1); MappingIndex >= 0; --MappingIndex)
	{
		FGameplayTagPropertyMapping& Mapping = InputPropertyMappings[MappingIndex];

		if (Mapping.TagToMap.IsValid())
		{
			FProperty* Property = Owner->GetClass()->FindPropertyByName(Mapping.PropertyName);
			if (Property && IsPropertyTypeValid(Property))
			{
				Mapping.PropertyToEdit = Property;
				continue;
			}
		}

		// Entry was invalid.  Remove it from the array.
		ensureAlwaysMsgf( false, TEXT("%s(). Property Invalid. ArraIndex: %i"), *FString(__FUNCTION__), MappingIndex );
		InputPropertyMappings.RemoveAtSwap(MappingIndex, 1, false);
	}
}

bool FGameplayTagPropertyMap::IsPropertyTypeValid(const FProperty* Property) const
{
	check(Property);
	return (Property->IsA<FBoolProperty>() || Property->IsA<FIntProperty>() || Property->IsA<FFloatProperty>());
}

void FGameplayTagPropertyMap::ResetAllValuesToZero()
{
	for(auto& Mapping : InputPropertyMappings)
	{
		if (const FBoolProperty* BoolProperty = CastField<const FBoolProperty>(Mapping.PropertyToEdit.Get()))
		{
			BoolProperty->SetPropertyValue_InContainer(Owner, false);
		}
	}
}

#if WITH_EDITOR

EDataValidationResult FGameplayTagPropertyMap::IsDataValid(UObject* ContainingAsset, TArray<FText>& ValidationErrors)
{
	UClass* OwnerClass = ((ContainingAsset != nullptr) ? ContainingAsset->GetClass() : nullptr);
	if (!OwnerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("FGameplayTagBlueprintPropertyMap: IsDataValid() called with an invalid Owner."));
		return EDataValidationResult::Invalid;
	}

	for (const FGameplayTagPropertyMapping& Mapping : InputPropertyMappings)
	{
		if (!Mapping.TagToMap.IsValid())
		{
			ensureAlwaysMsgf( false, TEXT("Error") );
		}

		if (FProperty* Property = OwnerClass->FindPropertyByName(Mapping.PropertyName))
		{
			if (!IsPropertyTypeValid(Property))
			{
				ensureAlwaysMsgf( false, TEXT("Error") );
			}
		}
		else
		{
			ensureAlwaysMsgf( false, TEXT("Error") );
		}
	}

	return ((ValidationErrors.Num() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}

#endif


