using UnrealBuildTool;

public class LumeGame: ModuleRules
{
    public LumeGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                "LumeGame"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "AIModule",
                "Core",
                "CommonLoadingScreen",
                "CoreOnline",
                "CoreUObject",
                "Engine",
                "GameplayTags",
                "GameplayTasks",
                "GameplayAbilities",
                "GameplayCameras",
                "GameFeatures",
                "ModularGameplay",
                "ModularGameplayActors",
                "Niagara",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "CommonUI",
                "CommonInput",
                "CommonGame",
                "CommonUser",
                "EnhancedInput",
                "EngineSettings",
                "InputCore",
                "NetCore",
                "Slate",
                "SlateCore",
                "UMG",
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
            }
        );
    }
}
