# Changelog
---
### Bug Fixes:
Fixed “double-free” error in `ClientDebuglayer` destructor
### New Features:
Added `Timer` class
Added a file dialogue system

Added Changelog file
### Changes:
Moved ImGui Debug window to dedicated `ClientDebugLayer`
Changed `OrthographicCameraController` class’ `OnUpdate()` function to pan with RMB instead of WASD
`OrhtographicCameraController` now requires `Framebuffer`.
`OrhtographicCameraController` uses framebuffer for size and aspect ratio for projection and mouse panning
Changed controls of `ClientNetLayer` to ImGui buttons instead of keyboard controls
Removed all `termios` code from Client code as all controls are now handled through the engine
Changed text regarding average frame time and rate
Changed logging format
