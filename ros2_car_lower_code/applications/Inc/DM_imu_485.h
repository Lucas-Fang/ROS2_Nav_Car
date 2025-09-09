#ifndef __DM_IMU_485_H_
#define __DM_IMU_485_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "crc16.h"
#include "usart.h"
#include "dma.h"
#include "string.h"
#define UART_RX_BUF_LEN 256

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

class DM_IMU{
	public:
	UART_HandleTypeDef *config_uart;
	DMA_HandleTypeDef  *config_dma_rx;
	imu_t imu;

	DM_IMU(UART_HandleTypeDef* uart, DMA_HandleTypeDef* dma) 
        : config_uart(uart), config_dma_rx(dma){}

	void HAL_UART_IDLE_Callback();
	void parse_imu_data(uint8_t *buf, uint16_t len);
	void myUART_DMA_init();

	private:
	uint8_t uart_rx_buf[256];  // DMA接收缓冲区
	uint8_t imu_frame[19];                 // 存放解析到的完整帧

};
extern DM_IMU dm_imu;

#ifdef __cplusplus
}
#endif
#endif