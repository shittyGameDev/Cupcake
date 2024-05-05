// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cupcake : ModuleRules
{
	public Cupcake(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "UMGEditor" });
		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG", 
			"Slate", 
			"SlateCore",
			"Niagara"
		});
	}
}
