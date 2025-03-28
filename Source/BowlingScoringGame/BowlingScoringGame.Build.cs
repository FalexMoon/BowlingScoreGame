// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BowlingScoringGame : ModuleRules
{
	public BowlingScoringGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Slate", "SlateCore" });
	}
}
