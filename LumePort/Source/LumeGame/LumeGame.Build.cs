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
                "PhysicsCore",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "AnimGraphRuntime",
                "CommonUI",
                "CommonInput",
                "CommonGame",
                "CommonUser",
                "EnhancedInput",
                "EngineSettings",
                "GameplayMessageRuntime",
                "InputCore",
                "NetCore",
                "IrisCore",
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
