#ifndef _ROS_SERIAL_H__
#define _ROS_SERIAL_H__

#include "usr_main.h"

#define SEND_HEAD_LENGTH 2
#define SEND_PAYLOAD_LENGTH 28
#define SEND_BAG_LENGTH (SEND_HEAD_LENGTH + SEND_PAYLOAD_LENGTH)
#define REC_HEAD_LENGTH 2
#define REC_PAYLOAD_LENGTH 24
#define REC_BAG_LENGTH (REC_HEAD_LENGTH + REC_PAYLOAD_LENGTH)

typedef union{
    uint8_t data[REC_BAG_LENGTH];
    struct 
    {
        uint8_t header[REC_HEAD_LENGTH];
        union
        {
            uint8_t pay_load[REC_PAYLOAD_LENGTH];
            struct 
            {
                float vx_set;
                float vy_set;
                float vw_set;
                float x_set;
                float y_set;
                float w_set;
            }__attribute__((packed));
        };
    }__attribute__((packed));
}__attribute__((packed))ros_driver_recData;

typedef union
{
    uint8_t data[SEND_BAG_LENGTH];
    struct
    {
        uint8_t header[SEND_HEAD_LENGTH];
        union
        {
            uint8_t payload[SEND_PAYLOAD_LENGTH];
            struct
            {
                float pos_x;
                float pos_y;
                float w_z;
                float zangle;
                float xangle;
                float yangle;
                float point;
            }__attribute__((packed));
        };
    }__attribute__((packed));
}__attribute__((packed)) ros_driver_sendData;

typedef __IO struct
{
    float vx_set_;
    float vy_set_;
    float vw_set_;
    float x_set_;
    float y_set_;
    float w_set_;
    SemaphoreHandle_t xMutex_rec_data;
} JoyRecData_;

typedef __IO struct
{
    float pos_x_;
    float pos_y_;
    float zangle_;
    float xangle_;
    float yangle_;
    float point_;
    SemaphoreHandle_t xMutex_send_data;
} JoySendData_;

extern uint8_t RxBuffer[REC_BAG_LENGTH*2] ;
extern ros_driver_sendData ros_dataBag_;
extern JoySendData_ joy_send_data_;

void SendToROS(ros_driver_sendData *ros_dataBag_,JoySendData_ *joy_send_data_);
void rosDecode();
void serialInit();

#endif