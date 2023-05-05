CFLAGS += \
  -DSTM32F303xC \
  -DHSE_VALUE=16000000U \
  -DBOARD_UF2_DEVICE_TYPE_ID=$(UF2_DEVICE_TYPE_ID)

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f303xc.s

SRC_C += \
  ports/stm32f3/boards/_drop/hub.c \
  $(ST_HAL_DRIVER)/Src/stm32f3xx_hal_adc.c \
  $(ST_HAL_DRIVER)/Src/stm32f3xx_hal_adc_ex.c


# For flash-jlink target
JLINK_DEVICE = stm32f303vc

flash: flash-stflash
erase: erase-stflash
