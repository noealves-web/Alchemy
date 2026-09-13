# Fitzroy Viewer — MVP

Fitzroy Viewer is a lightweight, practical Windows viewer for Second Life,
built from the current Alchemy Viewer codebase. The first testing audience is
a closed team, and the minimum performance target is integrated Intel UHD or
AMD Vega graphics with 8–16 GB of system memory.

## Product principles

1. PBR stays available in every graphics profile.
2. Common choices take one click; expert controls remain accessible.
3. The viewer never changes the local environment without a visible user action.
4. Performance defaults prioritize stable frame pacing over peak screenshot quality.
5. Upstream Alchemy and Linden Lab compatibility is preserved.

## MVP graphics profiles

| Fitzroy profile | Alchemy level | Intended use |
| --- | ---: | --- |
| Light | Low-Mid (1) | Integrated graphics, social use, busy regions |
| Balanced | Mid-High (3) | Everyday use on mainstream hardware |
| PBR Max | Ultra (6) | Photography and powerful discrete GPUs |

The profiles use Alchemy's maintained feature table rather than duplicating
dozens of low-level settings. GPU-specific masks therefore continue to protect
unsupported hardware.

## MVP environment controls

The Quick Settings panel exposes Shared, Midday, Sunset, and Night. Shared
restores the parcel or region EEP. The other choices apply a local-only EEP sky
and do not alter what other residents see.

## Planned next increments

- Portuguese UI strings and first-run setup.
- Target-FPS auto mode with a visible status and manual override.
- Simple environment panel for exposure, gamma, haze, clouds, water, and time.
- Named user presets for graphics plus environment.
- Fitzroy application identity, unique viewer identifier, icons, installer, and
  update channel for the closed test team.
- Performance telemetry stored locally and exportable only by explicit action.

## Compliance gates before team distribution

- Preserve LGPL notices and publish the covered source corresponding to builds.
- Assign a unique viewer identifier; never impersonate an official viewer.
- Publish privacy and support information before distributing installers.
- Confirm compliance with the current Linden Lab Third-Party Viewer Policy.
- Do not add permission bypasses or unrestricted content export.
