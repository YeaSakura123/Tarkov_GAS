// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class tarkov_gas : ModuleRules
{
	public tarkov_gas(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"UMG",
			"ModelViewViewModel"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "AnimGraphRuntime" });

	}
}
