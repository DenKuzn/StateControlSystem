// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

#include "MyExtensionEditor.h"

#include "Framework/GameplayTagPropertyMappingDetails.h"

void FMyExtensionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout( "GameplayTagPropertyMapping", FOnGetPropertyTypeCustomizationInstance::CreateStatic( &FGameplayTagPropertyMappingDetails::MakeInstance ) );

}

void FMyExtensionEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("GameplayTagPropertyMapping");
}

IMPLEMENT_MODULE(FMyExtensionEditorModule, MyExtensionEditor)