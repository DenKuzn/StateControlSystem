// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class MyExtensionEditor : ModuleRules
	{
		public MyExtensionEditor(ReadOnlyTargetRules Target) : base(Target)
		{
			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"MyExtension",
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"AssetRegistry",
					"DeveloperSettings",
					"Engine",
					"GameFeatures",
					"EditorSubsystem",
					"UnrealEd",
					"Projects",
					"EditorFramework",
					"Slate",
					"SlateCore",
					"PropertyEditor",
					"SharedSettingsWidgets",
					"Json",

					"InputCore",
				}
			);
		}
	}
}