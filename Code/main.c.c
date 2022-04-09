/**
 * ProjectTestV13
 * Gregory Sveinbjornson
 * Version: Final Version
 * April 5, 2022
 * 
 * Notes: All the code is in the main.c file and main.h file,
 * I am thinking about seperating it but everything is working
 * and I don't want anything to break
 * 
 */

#include "main.h"

//declaring init functions
void SystemClock_Config(void);
static void GPIO_Init(void);

int main(void)
{

  HAL_Init();//init functions
  SystemClock_Config();
  GPIO_Init();

  //placeholder sensor value
  uint8_t sensor_val = 0;

  //making sure alarm pins are set to low
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, 0);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);

  while (1)//loop
  {
      //reading the sensor pin
		  sensor_val = HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin);

      //if there is motion
		  if(sensor_val == 1){
        //sets off the alarm
			  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, 1);
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);

		  }else{
        //once the motion has stopped, so does the alarm
        //the system runs for as long as the knob on the sensor
        //allows(1s-15min)
			  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, 0 );
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);

		  }


  }
 
}

//initializing system clock
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
 
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

}


static void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //enabling ports
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  //setting default level
  HAL_GPIO_WritePin(GPIOA, Buzzer_Pin|LD2_Pin, 0);

  //enabling button for test
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  //enabling future keypad pin
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  //enabling buzzer/led/testing led
  GPIO_InitStruct.Pin = Buzzer_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //enabling sensor and on off pin
  GPIO_InitStruct.Pin = Sensor_Pin|On_off_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //inturrupt init
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}


