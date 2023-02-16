CFLAGS += \
  -DSTM32F303xC \
  -DHSE_VALUE=16000000U

SRC_S += \
  $(ST_CMSIS)/Source/Templates/gcc/startup_stm32f303xc.s

SRC += hub.c

# For flash-jlink target
JLINK_DEVICE = stm32f303vc

flash: flash-stlink
erase: erase-stlink
