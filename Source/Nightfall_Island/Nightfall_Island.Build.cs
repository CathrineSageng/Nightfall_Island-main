// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Nightfall_Island : ModuleRules
{
	public Nightfall_Island(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "AdvancedWidgets"});
	}
}
