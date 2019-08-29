# RenderFramework
A 3D rendering framework, developed as needed for my projects

# Goals
1. Support common 3D rendering techniques
2. Be flexible/configurable enough for my personal projects
3. Be easier to build and customise than larger scene graphs/libraries
4. Be (mostly) free of legacy technologies. Minimum OpenGL 4.5/GLES 3.0

# Supported Platforms
Platform|Compiler|Status
---|---|---
Linux|GCC 7+|Primary testing platform
Linux|Clang|Untested, should work
Windows|VS2017|Secondary testing platform, primarily in the context of VR development
Android||Untested, should work, no java api available
Emscripten||Early proof of concept worked well, otherwise untested. Engine assumes OpenGL 4.5 currently so won't work out of the box.

# APIs
Feature|Status|Notes
---|---|---
C++|Present, unstable ABI|Primary API, all others would be wrappers around this
Java|Unplanned|Concepts proven
C#|Unplanned|Concepts proven, .Net Core is kinda cool
Python|Unplanned|Concepts proven, except OpenGL context creation
Javascript|Unplanned|Concepts partially proven through emscripten POC

# Feature Status
## Rendering pipeline
Feature|Status|Notes
---|---|---
OpenGL implementation|Complete for present features|To be extended as features are added.
GLES/WebGL implementation|Partially Planned|Largely same path as OpenGL, might already work
Vulkan implementation |0%|Unplanned at this time, opengl generally fast enough. Some of my boxes can't support Vulkan yet :'(
Shader pipeline|30%|Fixed to basic phong implementation currently, shader/texture sets to be integrated into scene graph
Render to Texture/Framebuffers|50%|Texture-backed framebuffer present for VR support, functional but limited

## Data Formats/Loaders
Feature|Status|Notes
---|---|---
Basic vector nodes|Complete|rfvector
rfvector primitives (cube,sphere,etc)|20%|Only Cube implemented so far
Common model formats|0%|Unplanned, would be useful
Raster loader|Complete|rfraster
PNG|Complete|
Texture loading support|20%?|HardCoded to PNG, 24/32-bit in Engine::loadTexture class

## 3rd Party Integration/Other Ideas
Feature|Status|Notes
---|---|---
OpenVR Integration|Not Integrated|Proof of concept demo functional, to be integrated into core engine
WebVR Integration|Unplanned|Would be cool, requires WebGL, javascript integration/emscripten support
