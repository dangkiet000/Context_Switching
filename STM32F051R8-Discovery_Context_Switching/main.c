/* Includes ------------------------------------------------------------------*/

#include "stm32f0xx.h"
#include "os.h"

#define LED_GPIO_ENABLE_CLK()	RCC->AHBENR |= RCC_AHBENR_GPIOAEN
#define LED_GPIOx		GPIOA
#define LED_GPIO_PIN		5



/** @addtogroup STM32F0_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BSRR_VAL        0x0300

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef        GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void task1_handler(void);
static void task2_handler(void);
static void task3_handler(void);
/* Private functions ---------------------------------------------------------*/

void HardFault_Handler(void)
{
	while(1);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Initialize task stacks: */
	static os_stack_t stack1[128];
	static os_stack_t stack2[128];
	static os_stack_t stack3[128];
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	os_init();

	os_task_init(&task1_handler, stack1, 128);
	os_task_init(&task2_handler, stack2, 128);
	os_task_init(&task3_handler, stack3, 128);
	
	/* Context switch every 100 milliseconds */
	os_start(SystemCoreClock/10);

  /* Enable global interrupt */
  __enable_irq();

  while (1)
  {

  }
}

static void task1_handler(void)
{
	while (1) 
  {
		__disable_irq();
    GPIOC->ODR ^= GPIO_Pin_8;
		__enable_irq();

    /* Delay 10*Tick = 1000ms */
		SysTickDelay(10);
	}
}

static void task2_handler(void)
{
	while (1) 
  {
		__disable_irq();
    GPIOC->ODR ^= GPIO_Pin_9;
		__enable_irq();

    /* Delay 2*Tick = 200ms */
		SysTickDelay(2);
	}
}

static void task3_handler(void)
{
	while (1) 
  {

		SysTickDelay(20);
	}
}







#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif