// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VerticalSlice : ModuleRules
{
	public VerticalSlice(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemSteam", "GameplayTags", "GameplayAbilities", "GameplayTasks", "Niagara" });
		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
