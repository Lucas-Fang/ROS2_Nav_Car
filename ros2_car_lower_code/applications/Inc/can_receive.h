#ifndef __CAN_RECEIVE_H_
#define __CAN_RECEIVE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "can.h"
#include "main.h"
#include "struct_typedef.h"

typedef struct
{
	float pitch;
	float roll;
	float yaw;

	float gyro[3];
	float accel[3];
	
	float q[4];

	float cur_temp;

}imu_t;


class CAN_BUS
{
    public:

    class BSP
    {
    public:
        void CAN_Start(CAN_HandleTypeDef *hcan);
        void Filter_Init(CAN_HandleTypeDef *hcan);
    }bsp;

    class DM_IMU
    {
    public:
        imu_t imu;

        void IMU_RequestData(CAN_HandleTypeDef* hcan,uint16_t can_id,uint8_t reg);
        void IMU_UpdateAccel(uint8_t* pData);
        void IMU_UpdateGyro(uint8_t* pData);
        void IMU_UpdateEuler(uint8_t* pData);
        void IMU_UpdateQuaternion(uint8_t* pData);
        void IMU_UpdateData(uint8_t* pData);
    }dm_imu;
};


extern CAN_BUS can_bus;
#ifdef __cplusplus
}
#endif
	
#endif