$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$build = Join-Path $root '..\..\..\build\windows'
$package = Join-Path $root '..\..\..\dist\Otuber'

cmake -S $PSScriptRoot -B $build
cmake --build $build --config Release

New-Item -ItemType Directory -Force -Path $package | Out-Null
$exe = Join-Path $build 'Release\otuber_windows_preview.exe'
Copy-Item $exe $package -Force

@"
Otuber Preview

Run otuber_windows_preview.exe to start the Windows preview application.
"@ | Set-Content (Join-Path $package 'README.txt') -Encoding UTF8

$zip = Join-Path (Split-Path $package -Parent) 'Otuber-Windows-Preview.zip'
if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path "$package\*" -DestinationPath $zip
Write-Host "Created $zip"
