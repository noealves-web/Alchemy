# Fitzroy Viewer 2026 — Lightweight PBR Photo Viewer

## Product goal

Build Fitzroy Viewer 2026 for Windows 10/11 x64 with current Second Life PBR compatibility while
combining a low-overhead everyday mode with a dedicated, lossless photo mode.
The viewer should remain usable on modest Windows PCs and expose environment
controls in a photographer-oriented workflow.

Alchemy is the upstream base. It already tracks the modern Second Life render
pipeline, uses LGPL-2.1-compatible source, and contains the EEP/environment and
snapshot systems that this project needs.

## Design rules

1. Do not obtain speed by lowering saved snapshot quality.
2. Separate interactive viewport quality from capture quality.
3. Make expensive effects adaptive and reversible.
4. Preserve PBR materials, reflections, alpha behavior, and scene lighting.
5. Keep the default interface compact; advanced controls remain one click away.
6. Measure frame time, VRAM pressure, texture backlog, and scene complexity.

## Runtime profiles

### Light

- PBR remains enabled.
- Dynamic draw distance and avatar complexity react to frame time.
- Reflection probe updates are rate-limited while the camera is moving.
- Shadow distance and resolution are reduced before texture resolution.
- Background media, animated previews, and nonessential UI work are suspended.
- Texture memory follows a conservative VRAM budget without forcing blurry
  snapshots.

### Balanced

- Stable everyday defaults.
- Full PBR material rendering.
- Moderate shadows, reflections, antialiasing, and avatar detail.
- Adaptive logic intervenes only when the configured frame-time target is
  missed for a sustained interval.

### Photo

- Freezes adaptive degradation for repeatable results.
- Raises shadow, reflection, texture, and antialiasing quality for capture.
- Supports high-resolution off-screen snapshots independent of window size.
- Restores the previous runtime profile after capture.
- Offers PNG output by default, with JPEG quality explicitly selectable.

## Photo workspace

The photo workspace should place the following controls in one dockable panel:

- camera position, focal length/FOV, roll, and saved camera slots;
- exposure, gamma, white balance, and scene tonemapping controls;
- sun/moon direction, time of day, cloud, haze, ambient light, and water;
- shadow quality, reflection quality, depth of field, and antialiasing;
- aspect-ratio guides, rule-of-thirds, center guide, and safe margins;
- before/after environment comparison;
- local presets that never alter the region or parcel environment;
- snapshot size, format, alpha background, metadata, and output folder.

## Environment workflow

Environment editing is split into three levels:

1. **Quick look** — exposure, sun angle, haze, cloud, and water reflection.
2. **Studio** — complete fixed-sky and fixed-water controls with saved presets.
3. **Day cycle** — timeline editing and preview without changing the region.

Every edit is applied to the local/edit environment first. Publishing to a
parcel or region remains a separate permission-aware action.

## Performance architecture

The performance controller must target frame time rather than a vague quality
level. It samples CPU frame time, GPU frame time when available, VRAM pressure,
texture fetch backlog, and camera motion. Changes use hysteresis so settings do
not oscillate.

Recommended degradation order:

1. lower reflection update frequency;
2. reduce shadow distance;
3. reduce non-focused avatar complexity;
4. reduce far-object detail and draw distance;
5. reduce expensive screen-space effects;
6. reduce viewport render scale as a last resort.

Texture resolution and snapshot resolution are not early degradation targets.

## Delivery phases

### Phase 0 — reproducible Windows 10/11 x64 build

- Keep the upstream `develop` branch as the synchronization source.
- Build the open-source x64 variant with Visual Studio 2026.
- Preserve the original compiler error in CI logs.
- Produce an unsigned private test installer and checksum.

### Phase 1 — Fitzroy-branded technical baseline

- Use the Fitzroy Viewer 2026 name, channel, isolated application identifiers,
  and the camera/film/photo magic artwork for the executable and installer.
- Confirm login, inventory, teleport, voice, media, PBR, EEP, and snapshots.
- Add a clean migration path so settings do not overwrite Alchemy or another
  installed viewer.

### Phase 2 — profiles and telemetry panel

- Implement Light, Balanced, and Photo profiles.
- Add live frame-time and VRAM indicators.
- Add reversible adaptive-quality control.

### Phase 3 — photo workspace

- Consolidate camera, EEP, render, and snapshot controls.
- Add local preset import/export and before/after comparison.
- Add high-resolution capture with automatic profile restoration.

### Phase 4 — optimization and validation

- Benchmark fixed scenes on low-, mid-, and high-tier GPUs.
- Compare frame time, VRAM, texture convergence, and snapshot output.
- Test NVIDIA, AMD, and Intel drivers before wider distribution.

## Acceptance criteria for the first usable build

- Logs into the Second Life production grid.
- Renders glTF/PBR materials and reflection probes correctly.
- Loads and locally overrides EEP environments.
- Saves full-resolution PNG snapshots without unintended scaling.
- Installs beside Alchemy without sharing its settings directory.
- Shows the real compiler or linker error when CI fails.
