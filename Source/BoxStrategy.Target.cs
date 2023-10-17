// Copyright by Denis Kuznetsov. 2023. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BoxStrategyTarget : TargetRules
{
	public BoxStrategyTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("BoxStrategy");
	}
}
