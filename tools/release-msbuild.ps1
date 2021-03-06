﻿Push-Location
Set-Location $PSScriptRoot

$msbuild_exe = (Get-ChildItem -Path (Join-Path ${env:ProgramFiles(x86)} MSBuild) -Filter msbuild.exe -Recurse | Select-Object -Last 1).FullName

$proj_x86 = Get-Item -Path ..\build\tsuki-studio.vcxproj
$proj_x64 = Get-Item -Path ..\build64\tsuki-studio.vcxproj

.$msbuild_exe "$proj_x86" "/p:Configuration=Release"
.$msbuild_exe "$proj_x64" "/p:Configuration=Release"

$dll_x86 = Get-Item -Path ..\build\Release\tsuki-studio.dll
$dll_x64 = Get-Item -Path ..\build64\Release\tsuki-studio.dll

mkdir .\release_zip -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\bin -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\bin\32bit -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\bin\64bit -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\data -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\data\obs-plugins -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\obs-plugins -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\obs-plugins\32bit -ErrorAction SilentlyContinue | Out-Null
mkdir .\release_zip\obs-plugins\64bit -ErrorAction SilentlyContinue | Out-Null

Copy-Item ..\README.md .\release_zip\README.txt
Copy-Item ..\deps\x86\lua53\lua53.dll .\release_zip\bin\32bit\lua53.dll
Copy-Item ..\deps\x64\lua53\lua53.dll .\release_zip\bin\64bit\lua53.dll
Copy-Item $dll_x86.FullName ".\release_zip\obs-plugins\32bit\$($dll_x86.Name)"
Copy-Item $dll_x64.FullName ".\release_zip\obs-plugins\64bit\$($dll_x64.Name)"
Copy-Item ..\lua .\release_zip\data\obs-plugins -Recurse
Rename-Item .\release_zip\data\obs-plugins\lua tsuki-studio

Compress-Archive .\release_zip\* .\release.zip -Force
Remove-Item .\release_zip -Recurse

Pop-Location