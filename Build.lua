-- premake5.lua
workspace "BattleShip"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Battleship-App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "Battleship-Core/Build-Core.lua"
group ""

include "Battleship-App/Build-App.lua"