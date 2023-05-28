#include "ros_serial.h"
#include <stdio.h>

uint8_t SEND_HEADER[2] = {0xAA, 0xBB};
uint8_t REC_HEADER[2] = {0xBB, 0xAA};
uint8_t RxBuffer[REC_BAG_LENGTH*2] = {0};

JoySendData_ joy_send_data_;
JoyRecData_ joy_rec_data_;
ros_driver_sendData ros_dataBag_;

void serialInit()
{
  joy_rec_data_.xMutex_rec_data = xSemaphoreCreateRecursiveMutex();
  joy_send_data_.xMutex_send_data = xSemaphoreCreateRecursiveMutex();
}

void SendToROS(ros_driver_sendData *ros_dataBag_ptr,JoySendData_ *joy_send_data_)
{
  ros_dataBag_ptr->header[0] = SEND_HEADER[0];
  ros_dataBag_ptr->header[1] = SEND_HEADER[1];
  vPortEnterCritical();
  // xSemaphoreTakeRecursive(joy_send_data_->xMutex_send_data, (TickType_t)10);
  // ros_dataBag_->point = joy_send_data_->point_;
  // ros_dataBag_ptr->point = 100.0;
  // xSemaphoreGiveRecursive(joy_send_data_->xMutex_send_data);
  // ros_dataBag_ptr->point = 100;
  ros_dataBag_ptr->point = ros_dataBag_.point;
  ros_dataBag_ptr->pos_x = 3.14;
  ros_dataBag_ptr->pos_y = 3.15;
  ros_dataBag_ptr->xangle = 0;
  ros_dataBag_ptr->yangle = 0;
  ros_dataBag_ptr->zangle = 0;
  vPortExitCritical(); 

  HAL_UART_Transmit(&huart8,(uint8_t *) (ros_dataBag_ptr), SEND_BAG_LENGTH, 10);

}

void rosDecode()
{
  for(int i = 0 ; i < REC_BAG_LENGTH*2 ; ++i)
  {
    printf("hahahahahaha\n");
    ros_driver_recData tempBag = *(ros_driver_recData*)(void*)(&(RxBuffer[i]));
    if(
        tempBag.header[0] == REC_HEADER[0]     &&
        tempBag.header[1] == REC_HEADER[1]
      ){
        printf("hihihi");
        joy_rec_data_.vx_set_ = tempBag.vx_set;
        joy_rec_data_.vy_set_ = tempBag.vy_set;
        joy_rec_data_.vw_set_ = tempBag.vw_set;
        break;          
      }
  }
}

