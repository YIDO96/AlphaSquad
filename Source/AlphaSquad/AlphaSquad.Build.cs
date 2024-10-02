// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AlphaSquad : ModuleRules
{
	public AlphaSquad(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"HeadMountedDisplay",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"UMG"
		});
	}
}
