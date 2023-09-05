// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

using UnrealBuildTool;

public class BoxStrategy : ModuleRules
{
	public BoxStrategy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PublicDependencyModuleNames.Add("GameplayTags");
		PublicDependencyModuleNames.Add("UMG");
		PublicDependencyModuleNames.Add("EnhancedInput");
		PublicDependencyModuleNames.Add("MyExtension");


		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
