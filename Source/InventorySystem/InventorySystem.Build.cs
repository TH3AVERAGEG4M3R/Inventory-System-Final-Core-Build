using UnrealBuildTool;

public class InventorySystem : ModuleRules
{
        public InventorySystem(ReadOnlyTargetRules Target) : base(Target)
        {
                PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
                IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
                
                PublicDependencyModuleNames.AddRange(
                        new string[]
                        {
                                "Core",
                                "CoreUObject",
                                "Engine"
                        }
                        );
                        
                
                PrivateDependencyModuleNames.AddRange(
                        new string[]
                        {
                                "Slate",
                                "SlateCore",
                                "UMG"
                        }
                        );
        }
}