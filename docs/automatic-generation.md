# Automatic Asset Generation

Otuber treats generated imagery as a preparation-time asset, not a per-frame rendering operation.

```text
Source Image
    -> Character / Part Analysis
    -> Occlusion Detection
    -> Asset Generation
    -> Cached Avatar Assets
    -> Mesh + Rig
    -> Realtime Renderer
```

Generated assets may include:

- hidden regions exposed when hair or other foreground parts move
- eyelid states for blinking
- intermediate eyelid states for smooth eye motion
- character-specific expressions
- occlusion fills and other missing regions

The generation pipeline is deliberately isolated from the realtime avatar engine. This allows different implementations to be used later, including classical image processing, local ML models, or optional external generation backends.

No image-generation model is invoked every frame. Generation happens during avatar preparation and its results are cached.
