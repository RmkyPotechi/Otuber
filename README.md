# Otuber

Live2D-free 2D avatar animation engine for OBS Studio.

Otuber is designed around one core idea: **turn ordinary 2D character art into a realtime OBS avatar without requiring a traditional Live2D model**.

## Design goals

- Native OBS source integration
- Support human and non-human characters
- Start from a single image when possible
- Automatic or assisted part separation
- Realtime mesh deformation
- Webcam, audio, mouse, keyboard, and manual motion inputs
- Physics for hair, cloth, accessories, floating characters, etc.
- Extensible avatar rigs and custom parameters

## Architecture

```text
Input System
  -> Motion State
  -> Avatar Rig
  -> Deformation + Physics
  -> GPU Renderer
  -> OBS Source
```

Tracking is intentionally separated from the avatar rig. Human face tracking is one input provider, not a requirement of the avatar system. This allows cats, robots, ghosts, mascots, and other characters to use custom mappings.

## Roadmap

1. OBS plugin skeleton
2. Image loading and rendering
3. Avatar model and parameter system
4. Mesh deformation
5. Webcam tracking input
6. Expression and motion mapping
7. Physics / secondary motion
8. Automatic part detection and rigging
9. User-friendly setup UI
10. Windows release and later cross-platform support

## Project status

Early development. The repository is intentionally being built in small, testable milestones.

## License

GPL-2.0-or-later.
