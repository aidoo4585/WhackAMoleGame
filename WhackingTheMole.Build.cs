// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WhackingTheMole : ModuleRules
{
	public WhackingTheMole(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
