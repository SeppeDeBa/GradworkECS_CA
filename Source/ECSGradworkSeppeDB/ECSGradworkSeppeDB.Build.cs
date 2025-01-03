// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ECSGradworkSeppeDB : ModuleRules
{
	public ECSGradworkSeppeDB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"EnhancedInput",
				"MassEntity",
				"StructUtils",
				"MassCommon",
				"MassMovement",
				"MassActors",
				"MassSpawner",
				"MassGameplayDebug",
				"MassSignals",
				"MassCrowd",
				"MassActors",
				"MassRepresentation",
				"MassReplication",
				"MassNavigation",
				"MassSimulation",
				"ZoneGraph",
				"MassGameplayDebug",
				"MassZoneGraphNavigation", 
			}
		);

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
