# Mesh Deformation

Otuber uses weighted vertex deformation rather than requiring a fixed human anatomy model.

```text
Parameter
   -> Mapping
   -> Region Transform
   -> Weighted Vertices
   -> Deformed Mesh
```

A `DeformationRegion` contains vertex indices and weights. A weight of `1` means the vertex receives the full local transform; a weight of `0` leaves it unchanged. Intermediate values create smooth falloff between moving and stationary areas.

This is the foundation for effects such as:

- moving the head while keeping the torso stable
- opening a mouth by moving only nearby vertices
- tilting an ear or antenna
- creating smooth hair movement
- applying character-specific deformation without assuming the character is human

The current implementation uses a simple local transform. More advanced deformers can be layered on later, including translation/rotation curves, mesh cages, masks, and physics-driven displacement.
