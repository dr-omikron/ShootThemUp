

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
			"EnhancedInput" 
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		PublicIncludePaths.AddRange(new string[]
		{
			"ShootThemUp/Public/Player", 
			"ShootThemUp/Public/Components", 
			"ShootThemUp/Public/Development",
			"ShootThemUp/Public/Weapon",
			"ShootThemUp/Public/UI"
		});
	}
}
