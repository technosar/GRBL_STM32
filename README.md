![GitHub Logo](https://github.com/technosar/media/blob/master/grbl32%20logo.png)
***

### Grbl v1.1 STM32F7 has been released !

***
``` 
New features :
  - 8 axis (X, Y, Z, A, B, C, U and V compatible with OpenPnP project)
  - external SPI EEPROM
  - M200 wait end of motion
  - M62 set outputs
  - M63 reset outputs
  - M66 wait input event
  - M100 map Z axis (select Z or U axis for OpenPnp project)
``` 
***

Grbl **STM32** is a no-compromise, high performance, low cost alternative to parallel-port-based motion control for CNC milling. It will run on a STM32F746 (Open746I-C Standard from **[Waveshare](https://www.waveshare.com/product/mcu-tools/stm32/open/open746i-c-standard.htm)**). 

The controller is written in highly optimized C utilizing every clever feature of the STM32-chips to achieve precise timing and asynchronous operation. It is able to maintain up to **200kHz** of stable, jitter free control pulses.

It accepts standards-compliant g-code and has been tested with the output of several CAM tools with no problems. Arcs, circles and helical motion are fully supported, as well as, all other primary g-code commands. Macro functions, variables, and most canned cycles are not supported, but we think GUIs can do a much better job at translating them into straight g-code anyhow.

Grbl includes full acceleration management with look ahead. That means the controller will look up to 18 motions into the future and plan its velocities ahead to deliver smooth acceleration and jerk-free cornering.

* Licensing: GRBL STM32F7XX is free software, released under the GPLv3 license.

* For more information and help, check out our **[Wiki pages!](https://github.com/technosar/GRBL_STM32/wiki)** If you find that the information is out-dated, please to help us keep it updated by editing it or notifying our community! Thanks!

* Lead Developer [_2011 - Current_]: Sungeun(Sonny) K. Jeon, Ph.D. (USA) aka @chamnit

* Lead Developer [_2009 - 2011_]: Simen Svale Skogsrud (Norway). aka The Originator/Creator/Pioneer/Father of Grbl.

* Lead Developer [_2019 - Current_]: TECHNOSAR

***

``` 
List of Supported G-Codes in Grbl v1.1 STM32F7XX:
  - Non-Modal Commands: G4, G10L2, G10L20, G28, G30, G28.1, G30.1, G53, G92, G92.1
  - Motion Modes: G0, G1, G2, G3, G38.2, G38.3, G38.4, G38.5, G80
  - Feed Rate Modes: G93, G94
  - Unit Modes: G20, G21
  - Distance Modes: G90, G91
  - Arc IJK Distance Modes: G91.1
  - Plane Select Modes: G17, G18, G19
  - Tool Length Offset Modes: G43.1, G49
  - Cutter Compensation Modes: G40
  - Coordinate System Modes: G54, G55, G56, G57, G58, G59
  - Control Modes: G61
  - Program Flow: M0, M1, M2, M30*
  - Coolant Control: M7*, M8, M9
  - Spindle Control: M3, M4, M5
  - Valid Non-Command Words: F, I, J, K, L, N, P, R, S, T, X, Y, Z
```

-------------
Grbl is an open-source project and fueled by the free-time of our intrepid administrators and altruistic users. If you'd like to donate, all proceeds will be used to help fund supporting hardware and testing equipment. Thank you!
