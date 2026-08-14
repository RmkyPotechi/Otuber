$ErrorActionPreference = 'Stop'

$repo = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$build = Join-Path $repo 'build\windows'
$dist = Join-Path $repo 'dist'
$package = Join-Path $dist 'Otuber'

$cmakeArgs = @()
if ($env:VCPKG_ROOT -and (Test-Path (Join-Path $env:VCPKG_ROOT 'scripts\buildsystems\vcpkg.cmake'))) {
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
    $cmakeArgs += '-DVCPKG_TARGET_TRIPLET=x64-windows'
}

cmake -S $PSScriptRoot -B $build @cmakeArgs
cmake --build $build --config Release

$exe = Join-Path $build 'Release\otuber_windows_preview.exe'
if (-not (Test-Path -LiteralPath $exe)) {
    throw "Windows preview executable was not produced: $exe"
}

if (Test-Path -LiteralPath $package) {
    Remove-Item -LiteralPath $package -Recurse -Force
}
New-Item -ItemType Directory -Force -Path $package | Out-Null
Copy-Item -LiteralPath $exe -Destination (Join-Path $package 'otuber_windows_preview.exe') -Force

# The OpenCV backend needs its runtime DLLs beside the executable.
if ($env:VCPKG_ROOT) {
    $opencvBin = Join-Path $env:VCPKG_ROOT 'installed\x64-windows\bin'
    if (Test-Path -LiteralPath $opencvBin) {
        Get-ChildItem -LiteralPath $opencvBin -Filter '*.dll' | Copy-Item -Destination $package -Force
    }
}

# The first tracker backend uses OpenCV's frontal-face cascade.
$cascade = Join-Path $package 'haarcascade_frontalface_default.xml'
Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/opencv/opencv/4.x/data/haarcascades/haarcascade_frontalface_default.xml' -OutFile $cascade
if (-not (Test-Path -LiteralPath $cascade)) {
    throw "Face detector model was not downloaded: $cascade"
}

@"
Otuber Preview

Run otuber_windows_preview.exe to start the Windows preview application.
The bundled OpenCV runtime and face detector model enable live face tracking.
"@ | Set-Content -LiteralPath (Join-Path $package 'README.txt') -Encoding UTF8

$zip = Join-Path $dist 'Otuber-Windows-Preview.zip'
if (Test-Path -LiteralPath $zip) { Remove-Item -LiteralPath $zip -Force }
Compress-Archive -Path (Join-Path $package '*') -DestinationPath $zip -Force

if (-not (Test-Path -LiteralPath $zip)) {
    throw "Windows preview package was not produced: $zip"
}

Write-Host "Created $zip"
Get-Item -LiteralPath $zip | Format-List FullName,Length
