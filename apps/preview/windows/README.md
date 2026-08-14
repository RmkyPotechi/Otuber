# Otuber Windows Preview

This is the first native Windows GUI milestone.

## Build with Visual Studio

Open a Developer Command Prompt for Visual Studio and run:

```bat
cmake -S apps\preview\windows -B build\windows
cmake --build build\windows --config Release
```

The executable is produced under the corresponding `build\windows\Release` directory.

This milestone intentionally uses only Win32/GDI for the window. Camera capture is kept as a separate backend so the UI does not become coupled to a specific capture library.

Next milestone:

- enumerate Windows cameras
- open a selected camera
- show live frames
- extract face landmarks
- map landmarks into `CameraState`
- drive the avatar preview
