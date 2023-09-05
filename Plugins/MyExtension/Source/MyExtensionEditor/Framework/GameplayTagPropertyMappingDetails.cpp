// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.


#include "GameplayTagPropertyMappingDetails.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "MyExtension/Framework/MyExtensionTypes.h"

TSharedRef<IPropertyTypeCustomization> FGameplayTagPropertyMappingDetails::MakeInstance()
{
	return MakeShareable(new FGameplayTagPropertyMappingDetails());
}

void FGameplayTagPropertyMappingDetails::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	NamePropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGameplayTagPropertyMapping, PropertyName));
	GuidPropertyHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGameplayTagPropertyMapping, PropertyGuid));

	FString SelectedPropertyName;
	NamePropertyHandle->GetValue(SelectedPropertyName);

	TArray<void*> RawData;
	GuidPropertyHandle->AccessRawData(RawData);

	FGuid SelectedPropertyGuid;
	if (RawData.Num() > 0)
	{
		SelectedPropertyGuid = *static_cast<FGuid*>(RawData[0]);
	}

	FProperty* FoundProperty = nullptr;

	TArray<UObject*> OuterObjects;
	NamePropertyHandle->GetOuterObjects(OuterObjects);

	for (UObject* ParentObject : OuterObjects)
	{
		if (!ParentObject)
		{
			continue;
		}

		for (TFieldIterator<FProperty> PropertyIt(ParentObject->GetClass()); PropertyIt; ++PropertyIt)
		{
			FProperty* Property = *PropertyIt;
			if (!Property)
			{
				continue;
			}

			// Only support booleans, floats, and integers.
			const bool bIsValidType = Property->IsA(FBoolProperty::StaticClass()) || Property->IsA(FIntProperty::StaticClass()) || Property->IsA(FFloatProperty::StaticClass());
			if (!bIsValidType)
			{
				continue;
			}

			// Only accept properties from a blueprint.
			if (!UBlueprint::GetBlueprintFromClass(Property->GetOwnerClass()))
			{
				continue;
			}

			// Ignore properties that don't have a GUID since we rely on it to handle property name changes.
			FGuid PropertyToTestGuid = GetPropertyGuid(Property);
			if (!PropertyToTestGuid.IsValid())
			{
				continue;
			}

			// Add the property to the combo box.
			PropertyOptions.AddUnique(Property);

			// Find our current selected property in the list.
			if (SelectedPropertyGuid == PropertyToTestGuid)
			{
				FoundProperty = Property;
			}
		}
	}

	// Sort the options list alphabetically.
	PropertyOptions.StableSort([](const TFieldPath<FProperty>& A, const TFieldPath<FProperty>& B) { return (A->GetName() < B->GetName()); });

	if ((FoundProperty == nullptr) || (FoundProperty != SelectedProperty) || (FoundProperty->GetName() != SelectedPropertyName))
	{
		// The selected property needs to be updated.
		OnChangeProperty(FoundProperty, ESelectInfo::Direct);
	}

	HeaderRow
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGameplayTagPropertyMappingDetails::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> TagToMapHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGameplayTagPropertyMapping, TagToMap));
	TSharedPtr<IPropertyHandle> PropertyToEditHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGameplayTagPropertyMapping, PropertyToEdit));

	// Add the FGameplay Tag first.
	StructBuilder.AddProperty(TagToMapHandle.ToSharedRef());

	// Add the combo box next.
	IDetailPropertyRow& PropertyRow = StructBuilder.AddProperty(StructPropertyHandle);
	PropertyRow.CustomWidget()
		.NameContent()
		[
			PropertyToEditHandle->CreatePropertyNameWidget()
		]

		.ValueContent()
		.HAlign(HAlign_Left)
		.MinDesiredWidth(250.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.Padding(0.0f, 0.0f, 2.0f, 0.0f)
			[
				SNew(SComboBox< TFieldPath<FProperty> >)
				.OptionsSource(&PropertyOptions)
				.OnGenerateWidget(this, &FGameplayTagPropertyMappingDetails::GeneratePropertyWidget)
				.OnSelectionChanged(this, &FGameplayTagPropertyMappingDetails::OnChangeProperty)
				.ContentPadding(FMargin(2.0f, 2.0f))
				.ToolTipText(this, &FGameplayTagPropertyMappingDetails::GetSelectedValueText)
				.InitiallySelectedItem(SelectedProperty)
				[
					SNew(STextBlock)
					.Text(this, &FGameplayTagPropertyMappingDetails::GetSelectedValueText)
				]
			]
		];
}

void FGameplayTagPropertyMappingDetails::OnChangeProperty(TFieldPath<FProperty> ItemSelected, ESelectInfo::Type SelectInfo)
{
	if (NamePropertyHandle.IsValid() && GuidPropertyHandle.IsValid())
	{
		SelectedProperty = ItemSelected;

		NamePropertyHandle->SetValue(GetPropertyName(ItemSelected));

		TArray<void*> RawData;
		GuidPropertyHandle->AccessRawData(RawData);

		if (RawData.Num() > 0)
		{
			FGuid* RawGuid = static_cast<FGuid*>(RawData[0]);
			*RawGuid = GetPropertyGuid(ItemSelected);
		}
	}
}

FGuid FGameplayTagPropertyMappingDetails::GetPropertyGuid(TFieldPath<FProperty> Property) const
{
	FGuid Guid;

	if (Property != nullptr)
	{
		UBlueprint::GetGuidFromClassByFieldName<FProperty>(Property->GetOwnerClass(), Property->GetFName(), Guid);
	}

	return Guid;
}

FString FGameplayTagPropertyMappingDetails::GetPropertyName(TFieldPath<FProperty> Property) const
{
	return (Property != nullptr ? Property->GetName() : TEXT("None"));
}

TSharedRef<SWidget> FGameplayTagPropertyMappingDetails::GeneratePropertyWidget(TFieldPath<FProperty> Property)
{
	return SNew(STextBlock).Text(FText::FromString(GetPropertyName(Property.Get())));
}

FText FGameplayTagPropertyMappingDetails::GetSelectedValueText() const
{
	return FText::FromString(GetPropertyName(SelectedProperty));
}