#include "DM_imu_485.h"

DM_IMU dm_imu(&huart4,&hdma_uart4_rx);

void DM_IMU::HAL_UART_IDLE_Callback(){
/*
    你用 HAL_UART_Receive_DMA(&huart1, uart_rx_buf, UART_RX_BUF_LEN) 启动 DMA 后，
    DMA 会把 串口收到的每个字节自动搬运到 uart_rx_buf[] 里面。
    UART_RX_BUF_LEN 是你配置的缓冲区大小，比如 256。
    DMA 内部有一个 剩余计数器 (NDTR)，它表示还有多少字节没接收完。
*/   
    uint32_t len = UART_RX_BUF_LEN - __HAL_DMA_GET_COUNTER(config_uart->hdmarx);

    HAL_UART_DMAStop(config_uart);     // 停止DMA接收

     // 在 uart_rx_buf[0..len-1] 中找到协议帧
    parse_imu_data(uart_rx_buf, len);

    HAL_UART_Receive_DMA(config_uart, uart_rx_buf, UART_RX_BUF_LEN);    // 重新启动DMA接收

}

void DM_IMU::parse_imu_data(uint8_t *buf, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
    // 找到包头
        if (buf[i] == 0x55 && i+18 < len && buf[i+1] == 0xAA && buf[i+18] == 0x0A)
        {
            memcpy(imu_frame, &buf[i], 19);

    // 校验 CRC16
            if (Get_CRC16(imu_frame, 16) == (imu_frame[16] | (imu_frame[17]<<8)))
            {//
                // int32_t roll  = (imu_frame[4] | (imu_frame[5]<<8) | (imu_frame[6]<<16) | (imu_frame[7]<<24));
                // int32_t pitch = (imu_frame[8] | (imu_frame[9]<<8) | (imu_frame[10]<<16) | (imu_frame[11]<<24));
                // int32_t yaw   = (imu_frame[12] | (imu_frame[13]<<8) | (imu_frame[14]<<16) | (imu_frame[15]<<24));
               
                memcpy(&imu.roll,&imu_frame[4],4);
                memcpy(&imu.pitch,&imu_frame[8],4);
                memcpy(&imu.yaw,&imu_frame[12],4);
                
                //CRC16 的校验数据域为 DATA[0]至 DATA[15]，CRC 校验程序请务必使用本说明书附录里的代码。
                // imu.roll = *((float *)&roll);
                // imu.pitch = *((float *)&pitch);
                // imu.yaw = *((float *)&yaw);
                // TODO: 保存到全局变量，或者发到任务队列
            }
            i += 18;     // 跳过一帧
        }
    }
}
void DM_IMU::myUART_DMA_init(){

    HAL_UART_Receive_DMA(config_uart, uart_rx_buf, UART_RX_BUF_LEN); 
    __HAL_UART_ENABLE_IT(config_uart, UART_IT_IDLE); // 使能空闲中断
}

extern "C"
void DMA2_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel3_IRQn 0 */

  /* USER CODE END DMA2_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(dm_imu.config_dma_rx);
  /* USER CODE BEGIN DMA2_Channel3_IRQn 1 */
  if (__HAL_UART_GET_FLAG(dm_imu.config_uart, UART_FLAG_IDLE) != RESET)
  {
      __HAL_UART_CLEAR_IDLEFLAG(dm_imu.config_uart); // 清除空闲中断标志
      dm_imu.HAL_UART_IDLE_Callback();    //用用户函数
  }
  /* USER CODE END DMA2_Channel3_IRQn 1 */
}