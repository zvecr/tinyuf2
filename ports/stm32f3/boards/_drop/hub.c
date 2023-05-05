#include "board_api.h"
#include "stm32f3xx_hal.h"

void board_wait_ms(uint16_t ms);

#define S_UP        B12
#define E_UP_N      B13
#define S_DN1       B14
#define E_DN1_N     B15
#define E_VBUS_1    A8
#define E_VBUS_2    A15
#define SRC_1       B8
#define SRC_2       B9
#define HUB_RESET_N D2
#define IRST        B1
#define SDB_N       B2
#define C1_A5_SENSE A4
#define C1_B5_SENSE A7
#define C2_A5_SENSE C4
#define C2_B5_SENSE B0

void HAL_GPIO_SetOutput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
};

void HAL_GPIO_SetInput(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
};

void HAL_GPIO_SetAnalog(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
};

uint32_t HAL_ADC_Read(ADC_HandleTypeDef* hadc, uint32_t channel) {
  ADC_ChannelConfTypeDef sConfig = {
    .Channel = channel,
    .Rank = ADC_REGULAR_RANK_1,
    .SamplingTime = ADC_SAMPLETIME_601CYCLES_5,
  };

  HAL_ADC_ConfigChannel(hadc, &sConfig);

  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 1000);

  uint32_t ret = HAL_ADC_GetValue(hadc);

  HAL_ADC_Stop(hadc);

  return ret;
}

void board_dfu_init_extra(void) {
    ADC_HandleTypeDef adc2 = {0};
    ADC_HandleTypeDef adc3 = {0};

    adc2.Instance = ADC2;
    adc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc2.Init.Resolution = ADC_RESOLUTION_12B;
    adc2.Init.ScanConvMode = ENABLE;
    adc2.Init.ContinuousConvMode = ENABLE;
    adc2.Init.DiscontinuousConvMode = DISABLE;
    adc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc2.Init.NbrOfConversion = 1;
    adc2.Init.DMAContinuousRequests = DISABLE;
    adc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    adc3.Instance = ADC3;
    adc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    adc3.Init.Resolution = ADC_RESOLUTION_12B;
    adc3.Init.ScanConvMode = ENABLE;
    adc3.Init.ContinuousConvMode = ENABLE;
    adc3.Init.DiscontinuousConvMode = DISABLE;
    adc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc3.Init.NbrOfConversion = 1;
    adc3.Init.DMAContinuousRequests = DISABLE;
    adc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  __HAL_RCC_ADC12_CLK_ENABLE();
  __HAL_RCC_ADC34_CLK_ENABLE();

  // initial state
  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_4);
  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_7);
  HAL_GPIO_SetOutput(GPIOC, GPIO_PIN_4);
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);

  HAL_GPIO_SetOutput(GPIOD, GPIO_PIN_2);

  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_13);
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_15);
  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_8);
  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_15);

  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_12);
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_14);
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_8);
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_9);

  HAL_ADC_Init(&adc2);
  HAL_ADC_Init(&adc3);

  board_timer_start(1);

  board_wait_ms(20);
  HAL_GPIO_SetAnalog(GPIOA, GPIO_PIN_4);
  HAL_GPIO_SetAnalog(GPIOA, GPIO_PIN_7);
  HAL_GPIO_SetAnalog(GPIOC, GPIO_PIN_4);
  HAL_GPIO_SetAnalog(GPIOB, GPIO_PIN_0);

  // reset hub
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
  board_wait_ms(100);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);

  board_wait_ms(500); // Allow power dissipation time on CC lines

  uint32_t v_con_1 = HAL_ADC_Read(&adc2, ADC_CHANNEL_1); // A4
  uint32_t v_con_2 = HAL_ADC_Read(&adc2, ADC_CHANNEL_4); // A7
  uint32_t v_con_3 = HAL_ADC_Read(&adc2, ADC_CHANNEL_5); // C4
  uint32_t v_con_4 = HAL_ADC_Read(&adc3, ADC_CHANNEL_12); // B0

  // TODO: dynamic port port configure logic?
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

  if ((v_con_1 + v_con_2) > (v_con_3 + v_con_4)) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
  } else {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
  }

  board_timer_stop();
}


void board_app_jump_extra(void) {
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  // TODO: Firmware breaks if we deinit adc? however also not doing it saves 0.5K
  // HAL_ADC_DeInit(&adc2);
  // HAL_ADC_DeInit(&adc3);
}
