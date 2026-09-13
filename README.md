<img alt="Fitzroy Viewer 2026 camera, film and photo icon" width="320" src="indra/newview/branding/fitzroy/fitzroy_viewer_2026.png">

# Fitzroy Viewer 2026

[![License: LGPL 2.1](https://img.shields.io/badge/License-LGPL_2.1-blue.svg)](LICENSE)
[![Windows](https://img.shields.io/badge/Windows-10%20%7C%2011-0078D4?logo=windows)](https://github.com/noealves-web/Alchemy/actions)

Fitzroy Viewer 2026 is a Windows 10/11 x64 client for [Second Life](https://secondlife.com), focused on PBR rendering, photography, environment controls, and a lightweight everyday profile. It is based on the open-source [Alchemy Viewer](https://github.com/AlchemyViewer/Alchemy), which in turn is derived from the official [Linden Lab viewer](https://github.com/secondlife/viewer).

## 📥 Download

Download the Windows x64 installer from [Fitzroy Viewer releases][releasesgh]. Windows 10 and Windows 11 are the only supported operating systems.

## 🔨 Building from source

The inherited Alchemy source uses CMake with vcpkg for dependency management. Platform setup, presets, build options, packaging, and troubleshooting are documented in [**doc/BUILD.md**](doc/BUILD.md).

```
git clone --recurse-submodules https://github.com/noealves-web/Alchemy.git fitzroy-viewer-2026
cd fitzroy-viewer-2026
python3 -m venv .venv && source .venv/bin/activate   # Windows: .\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
dotnet tool restore                                  # packaging only
cmake -S indra --preset <preset>                     # see BUILD.md for presets
cmake --build build-<OS>-<preset> --config Release
```

## Upstream and acknowledgements

Fitzroy preserves the license notices and source attribution of the projects it builds upon:

- [Alchemy Viewer](https://github.com/AlchemyViewer/Alchemy) and its contributors
- [Linden Lab](https://www.lindenlab.com/) and the [Second Life Viewer](https://github.com/secondlife/viewer) contributors
- The many open-source libraries that power the viewer (see [`indra/vcpkg.json`](indra/vcpkg.json))

## 🔗 Resources

- [Downloads][releasesgh]
- [Issue tracker](https://github.com/noealves-web/Alchemy/issues)
- [Upstream Alchemy project](https://github.com/AlchemyViewer/Alchemy)

## 📜 License

Fitzroy Viewer 2026 is licensed under the [GNU Lesser General Public License v2.1](LICENSE), inherited from Alchemy Viewer and the upstream Linden Lab viewer. Existing copyright notices remain with their respective authors.

[releasesgh]: https://github.com/noealves-web/Alchemy/releases
