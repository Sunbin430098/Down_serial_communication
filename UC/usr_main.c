#include "usr_main.h"

void lightTask()
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
        osDelay(500);
        // printf("I:1");
    }
}

void joyTask()
{
    HAL_UART_Receive_DMA(&huart1,JoyStickReceiveData,18);
    for(;;)
    {
        osDelay(10);
    }
}

void serialTask()
{
    HAL_UART_Receive_IT(&huart8,(uint8_t *) (&RxBuffer), 2*REC_BAG_LENGTH);
    serialInit();
    for(;;)
    {
        SendToROS(&ros_dataBag_,&joy_send_data_);
        vTaskDelay(10);
    }
}

void lightTaskStart()
{
    osThreadDef(light, lightTask, osPriorityNormal, 0, 256);
	osThreadCreate(osThread(light), NULL);
}
void joyTaskStart()
{
    osThreadDef(light, joyTask, osPriorityNormal, 0, 512);
	osThreadCreate(osThread(light), NULL);
}
void serialTaskStart()
{
    osThreadDef(light, serialTask, osPriorityNormal, 0, 512);
	osThreadCreate(osThread(light), NULL);
}

void USER_INIT()
{
    lightTaskStart();
    serialTaskStart();
    joyTaskStart();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == huart1.Instance)
    {
        joyDecode();
    }
    else if(huart->Instance == huart8.Instance)
    {
        rosDecode();
        HAL_UART_Receive_IT(&huart8,(uint8_t *) (&RxBuffer), 2*REC_BAG_LENGTH);
    }

    if(HAL_UART_Receive_IT(&huart8,(uint8_t *) (&RxBuffer), 2*REC_BAG_LENGTH) != HAL_OK){
      __HAL_UART_CLEAR_OREFLAG(&huart8);
      HAL_UART_Receive_IT(&huart8,(uint8_t *) (&RxBuffer), 2*REC_BAG_LENGTH);
    }
}