# Otuber Avatar Format

## Purpose

The avatar format describes a 2D character independently from any tracking device. A model can therefore be driven by a webcam, microphone, mouse, keyboard, automation, or manual controls.

## Model structure

```text
Avatar
├── metadata
├── texture(s)
├── parts[]
├── parameters[]
├── mappings[]
└── physics[]
```

### Parts

A part is a drawable region of the character. Parts are semantic rather than tied to a human anatomy hierarchy.

Examples:

- `head`
- `eye_left`
- `eye_right`
- `mouth`
- `hair_front`
- `hair_back`
- `body`
- `ear_left`
- `antenna`
- `custom_*`

Each part may reference a mesh region and a texture region.

### Parameters

Parameters are normalized animation controls. They are not limited to human expressions.

Examples:

```text
head_x
head_y
head_rotation
eye_left_open
eye_right_open
mouth_open
body_tilt
hair_sway
ear_left
antenna
float
```

Parameter values use a normalized range where practical, allowing different input systems to map into the same rig.

### Mappings

A mapping converts an input motion value into one or more avatar parameters.

```text
Input -> Mapping -> Parameter -> Deformer / Physics
```

A mapping may define an input range, output range, smoothing, dead zone, multiplier, and optional easing.

This is what allows the same tracking signal to control different characters differently. For example, `mouth_open` can drive a human mouth, a robot display, or a ghost's expression.

## Example

```json
{
  "version": 1,
  "name": "Example Avatar",
  "parts": [
    {"id": "head", "type": "head"},
    {"id": "eye_left", "type": "eye"},
    {"id": "eye_right", "type": "eye"},
    {"id": "mouth", "type": "mouth"},
    {"id": "hair_front", "type": "hair"}
  ],
  "parameters": [
    {"id": "head_rotation", "min": -1, "max": 1, "default": 0},
    {"id": "mouth_open", "min": 0, "max": 1, "default": 0},
    {"id": "hair_sway", "min": -1, "max": 1, "default": 0}
  ],
  "mappings": []
}
```

The format is intentionally small in v1. More advanced rigging, masks, mesh deformation, physics graphs, and automatic rig metadata can be added without making the core input system human-specific.
