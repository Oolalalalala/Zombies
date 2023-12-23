workspace "Zombies"
    platforms { "Win64" }
    configurations { "Debug" , "Release", "Distribution" }
    startproject "Game"


filter { "platforms:Win64" }
    system "Windows"
    architecture "x86_64"


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    --include "Olala/vendor/imgui"
    --include "Olala/vendor/yaml-cpp"
group ""

include "Engine"
include "Game"