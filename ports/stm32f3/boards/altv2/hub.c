#include "board_api.h"
#include "stm32f3xx_hal.h"

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
#define C1_SENSE    A7
#define C2_SENSE    B0

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

void board_wait_ms(uint16_t ms);
// #define board_wait_ms(ms)

// void board_init_extra(void) {
void board_dfu_init_extra(void) {
  board_timer_start(1);

  // initial state
  HAL_GPIO_SetInput(GPIOA, GPIO_PIN_7);
  HAL_GPIO_SetInput(GPIOB, GPIO_PIN_0);

  // reset hub
  HAL_GPIO_SetOutput(GPIOD, GPIO_PIN_2);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
  board_wait_ms(100);
  HAL_GPIO_SetOutput(GPIOD, GPIO_PIN_2);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);

  board_wait_ms(500); // Allow power dissipation time on CC lines

  uint16_t v_con_1 = 0;
  uint16_t v_con_2 = 0;
  (void)v_con_1;
  (void)v_con_2;

  // TODO: dynamic port port configure logic?
  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_13);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);

  HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_15);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);

  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_8);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

  HAL_GPIO_SetOutput(GPIOA, GPIO_PIN_15);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);

  // force left?
  if (1) {
  // if (v_con_1 > v_con_2) {
    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_12);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_14);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_8);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_9);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0);
  } else {
    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_12);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_14);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_8);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);

    HAL_GPIO_SetOutput(GPIOB, GPIO_PIN_9);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1);
  }

  board_timer_stop();
}


void board_app_jump_extra(void) {
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
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
}
