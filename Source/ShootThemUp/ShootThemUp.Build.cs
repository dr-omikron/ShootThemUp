using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[]
		{
			"ShootThemUp/Public/Player",
			"ShootThemUp/Public/Components",
			"ShootThemUp/Public/Development",
			"ShootThemUp/Public/Weapon",
			"ShootThemUp/Public/Weapon/Components",
			"ShootThemUp/Public/UI",
			"ShootThemUp/Public/Animations",
			"ShootThemUp/Public/Pickups",
			"ShootThemUp/Public/AI",
			"ShootThemUp/Public/AI/Tasks",
			"ShootThemUp/Public/AI/EQS",
			"ShootThemUp/Public/AI/Decorators",
			"ShootThemUp/Public/Menu",
			"ShootThemUp/Public/Sound"
		});
	}
}