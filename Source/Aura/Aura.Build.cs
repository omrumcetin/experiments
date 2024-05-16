// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayTasks",
			"GameplayTags",
			"NavigationSystem",
			"Niagara"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"Aura",
			"Aura/Public"
		});
	}
}
