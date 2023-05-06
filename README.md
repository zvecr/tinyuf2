# TinyUF2 Bootloader

Supported devices:

* ALT v2
* CTRL v2
* SHIFT v2

## Update

Updates are provided in `.uf2` format on the [releases](https://github.com/zvecr/tinyuf2/releases/latest) page. These files can be flashed with the same process as QMK firmware binaries.

## Build and Flash

**Note:** These steps are provided primarily for recovery purposes.

| Device   | `BOARD`   |
|----------|-----------|
| ALT v2   | `altv2`   |
| CTRL v2  | `ctrlv2`  |
| SHIFT v2 | `shiftv2` |

### Setup

A working `arm-none-eabi-gcc` environment is required. Version `12.2.0` used in CI is known to provide compatible binaries.

```
make -C ports/stm32f3 BOARD=<board> get-deps
```

### Compile

To create self-update binaries:
```
make -C ports/stm32f3 BOARD=<board> clean all self-update
```

**Note:** Due to inconsistencies between compiler versions, care should be taken to ensure the compiled bootloader is not oversized.

### Flash

TODO:

* st-link hookup
* undo overwrite protection
  * CLI alternative to STM32CubeProgrammer?

Production files are provided in `.hex` format on the [releases](https://github.com/zvecr/tinyuf2/releases/latest) page. These files can be flashed with the following command.
```
st-flash --reset --format ihex write tinyuf2-altv2-0.0.1-production.hex
```

To compile/flash from source:
```
make -C ports/stm32f3 BOARD=<board> get-deps clean flash
```
