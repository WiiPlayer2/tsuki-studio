Push-Location
Set-Location $PSScriptRoot

$cmake_exe = (Get-ChildItem -Path (Join-Path $env:ProgramFiles CMake) -Filter cmake.exe -Recurse).FullName

mkdir ..\build -ErrorAction SilentlyContinue | Out-Null
mkdir ..\build64 -ErrorAction SilentlyContinue | Out-Null

Set-Location ..\build
. $cmake_exe -D "CMAKE_CONFIGURATION_TYPES=Debug;Release" -D BUILD_X64=OFF -G "Visual Studio 14 2015" ..

Set-Location ..\build64
. $cmake_exe -D "CMAKE_CONFIGURATION_TYPES=Debug;Release" -D BUILD_X64=ON -G "Visual Studio 14 2015 Win64" ..

Pop-Location