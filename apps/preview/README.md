# Otuber Preview

The preview application is intentionally dependency-light during the first milestone.

It runs the real Otuber mesh/deformer core and prints the resulting vertices. This gives us a deterministic executable for validating deformation before introducing a platform GUI or GPU backend.

## Build

```bash
cmake -S . -B build
cmake --build build
./build/otuber_preview
```

The next preview milestone will replace the text output with an actual window, texture loading, and interactive parameter controls.
