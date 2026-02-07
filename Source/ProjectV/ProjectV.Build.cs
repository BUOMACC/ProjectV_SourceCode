// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectV : ModuleRules
{
	public ProjectV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange( new string[] { "ProjectV" } );
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"MotionWarping",
			"Niagara",
			"PhysicsCore",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG",
			"NetCore",
			"CommonUI"
		});
	}
}
