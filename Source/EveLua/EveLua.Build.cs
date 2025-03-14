// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class EveLua : ModuleRules
{
	public EveLua(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// 添加 Lua 头文件路径
		string LuaIncludePath = Path.Combine(ModuleDirectory, "Lua\\Inc", "src");
		PublicIncludePaths.Add(LuaIncludePath);
		System.Console.WriteLine($"[LuaLog] Lua Include Path: {LuaIncludePath}");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// 添加 Lua 静态库文件路径
			string LibPath = Path.Combine(ModuleDirectory, "Lua", "Lib", "Lua.lib");
			PublicAdditionalLibraries.Add(LibPath);
			System.Console.WriteLine($"[LuaLog] Lua Library Path: {LibPath}");
		}
	}
}
