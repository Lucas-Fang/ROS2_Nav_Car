#include "can_receive.h"

CAN_BUS can_bus;

extern "C"
void HAL_CAN_RxFifo0MsgPendinngCallback(CAN_HandleTypeDef *hcan)
{		
    CAN_RxHeaderTypeDef rx_header;
	uint8_t pdata[8];
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&rx_header,pdata);
    
	// if(hcan->Instance==CAN1)
	// {		
		if(rx_header.StdId==0x11)  
            can_bus.dm_imu.IMU_UpdateData(pdata);
 	// }
}
