CFLAGS += \
  -DSTM32F303xC \
  -DHSE_VALUE=16000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f303xc.s

SRC_C += \
  $(ST_HAL_DRIVER)/Src/stm32f3xx_hal_adc.c \
  $(ST_HAL_DRIVER)/Src/stm32f3xx_hal_adc_ex.c

SRC += hub.c

# For flash-jlink target
JLINK_DEVICE = stm32f303vc

flash: flash-stflash
erase: erase-stflash
