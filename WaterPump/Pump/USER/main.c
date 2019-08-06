/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "control.h" 


/*
*7.17 : 1.增加看门狗  2.调整PI参数  3.control各个任务调度  
*       现象：板子正常工作
*
*
*
*
*
*
*
*/


void main(void)
{
  /* Infinite loop */
  BSP_Init();
  Run();
 
  
}

  
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
 