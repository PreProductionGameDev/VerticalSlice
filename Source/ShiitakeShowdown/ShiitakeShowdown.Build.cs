// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShiitakeShowdown : ModuleRules
{
	public ShiitakeShowdown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemSteam", "GameplayTags", "GameplayAbilities", "GameplayTasks", "Niagara", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
