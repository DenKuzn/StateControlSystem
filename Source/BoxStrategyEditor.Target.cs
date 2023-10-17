// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BoxStrategyEditorTarget : TargetRules
{
	public BoxStrategyEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("BoxStrategy");
	}
}
