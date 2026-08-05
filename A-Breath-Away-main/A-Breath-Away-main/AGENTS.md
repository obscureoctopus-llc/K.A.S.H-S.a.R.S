# AGENTS.md

This repository is a PlatformIO/Arduino project centered on Zigbee pilot-wire control for ESP32-C6/H2 targets.

## Quick Start For Agents

- Primary docs: [README.md](README.md)
- Home Assistant quirks/docs: [extras/homeassistant/README.md](extras/homeassistant/README.md)
- Example sketches:
  - [examples/VirtualPilotSerial/README.md](examples/VirtualPilotSerial/README.md)
  - [examples/VirtualPilotRgbLed/README.md](examples/VirtualPilotRgbLed/README.md)

## Project Shape

- Root [platformio.ini](platformio.ini) is the main PlatformIO configuration.
- Board-specific environments are defined as `[env:<name>]` sections.
- Current default env in root config is `presence_xiao_esp32c6_hlk_ld2410b`.
- Most runnable code is currently in example/test sketches:
  - [examples/VirtualPilotSerial/VirtualPilotSerial/VirtualPilotSerial.ino](examples/VirtualPilotSerial/VirtualPilotSerial/VirtualPilotSerial.ino)
  - [examples/VirtualPilotRgbLed/VirtualPilotRgbLed/VirtualPilotRgbLed.ino](examples/VirtualPilotRgbLed/VirtualPilotRgbLed/VirtualPilotRgbLed.ino)
  - [extras/tests/VirtualPilotTestWithLog/VirtualPilotTestWithLog/VirtualPilotTestWithLog.ino](extras/tests/VirtualPilotTestWithLog/VirtualPilotTestWithLog/VirtualPilotTestWithLog.ino)

## Common Commands

Run from repository root unless `-d` is provided.

- Build default env:
  - `pio run`
- Build a specific board env:
  - `pio run -e seeed_xiao_esp32c6`
- Upload for a specific env:
  - `pio run -e seeed_xiao_esp32c6 -t upload`
- Open serial monitor:
  - `pio device monitor -b 115200`
- Build an example project directly:
  - `pio run -d examples/VirtualPilotSerial -e seeed_xiao_esp32c6`

## Conventions Agents Should Preserve

- Keep `framework = arduino` and `monitor_speed = 115200` unless explicitly asked to change.
- Keep platform pinning to pioarduino/espressif32 when editing PlatformIO envs.
- For Zigbee router/coordinator environments, preserve ZCZR-related build flags and `zigbee_zczr.csv` partition usage.
- Prefer adding or editing an existing `[env:<name>]` block instead of replacing unrelated environments.

## Known Pitfalls

- There are two similarly named config files: [platformio.ini](platformio.ini) and [platformiio.ini](platformiio.ini). Verify which file a task refers to before editing.
- Example/test subfolders also contain their own `platformio.ini`; avoid changing all configs at once unless requested.

## Change Scope Guidance

- Keep changes minimal and board-scoped.
- Do not change hardware pin macros, Zigbee mode flags, or partition scheme values without explicit user direction.
- If behavior questions arise, reference existing examples before introducing new architecture.
