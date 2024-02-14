// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectEscape : ModuleRules
{
	public ProjectEscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "AIModule",
													"InputCore", "EnhancedInput", "NavigationSystem", "GameplayTasks"
													,"Niagara"
		});
	}
}
