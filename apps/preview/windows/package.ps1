$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$build = Join-Path $repo 'build\windows'
$dist = Join-Path $repo 'dist'
$package = Join-Path $dist 'Otuber'

cmake -S $PSScriptRoot -B $build
cmake --build $build --config Release

$exe = Join-Path $build 'Release\otuber_windows_preview.exe'
if (-not (Test-Path -LiteralPath $exe)) {
    throw "Windows preview executable was not produced: $exe"
}

New-Item -ItemType Directory -Force -Path $package | Out-Null
Copy-Item -LiteralPath $exe -Destination (Join-Path $package 'otuber_windows_preview.exe') -Force

@"
Otuber Preview

Run otuber_windows_preview.exe to start the Windows preview application.
"@ | Set-Content -LiteralPath (Join-Path $package 'README.txt') -Encoding UTF8

$zip = Join-Path $dist 'Otuber-Windows-Preview.zip'
if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }
Compress-Archive -Path (Join-Path $package '*') -DestinationPath $zip -Force

if (-not (Test-Path -LiteralPath $zip)) {
    throw "Windows preview package was not produced: $zip"
}

Write-Host "Created $zip"
Get-Item -LiteralPath $zip | Format-List FullName,Length
