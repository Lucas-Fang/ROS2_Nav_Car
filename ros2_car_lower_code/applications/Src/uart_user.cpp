#include "uart_user.h"

uint8_t rx_buffer[1];
remote_data receive_data;
extern uint8_t robot_mode;

extern "C"
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART5)
	{
		Receive_data(rx_buffer[0],DATA_LEN);
		
 	}

}

void BlueTooth_Send_Data(UART_HandleTypeDef *huart)
{
	uint8_t data_buf[7];
	data_buf[0] = 0xA5;
	data_buf[6] = 0x5A;
	
	// memcpy(&data_buf[1], &N5065_Data[1].speed_rpm, 4);
	data_buf[5] = SUMCRC(data_buf, 4);

	HAL_UART_Transmit(huart, data_buf, 7, 100);
}
//数据解析
void DataAnalysis(uint8_t *Data)
{
	robot_mode = Data[0];
	 receive_data.ch[0]=(int16_t)(Data[2]<<8|(int16_t)Data[1]);
     receive_data.ch[1]=(int16_t)(Data[4]<<8|(int16_t)Data[3]);
	 receive_data.ch[2]=(int16_t)(Data[6]<<8|(int16_t)Data[5]);
   
	// speed.vx=receive_data.ch[0];
	// speed.vy=receive_data.ch[1];
	// speed.vomega=receive_data.ch[2];


}
//和校验
uint8_t SUMCRC(uint8_t *Message,uint16_t DataLen)
{
	uint16_t i=1;
	uint8_t SUM=0x00;
	for(;i<=DataLen;i++)
	{
		SUM+=Message[i];
	}

	return SUM;

}
//接收数据
void Receive_data(uint8_t Bytedata,uint8_t Data_len)
{
	static uint8_t step=0;
	static uint8_t count=0,Buff[300]={0},*data_ptr;

	switch(step)
	{
		case 0:
		  if(Bytedata == 0xA5)
	  	{
  			step++;
				count=0;
				Buff[count++]=Bytedata;
				data_ptr=&Buff[count];
  		}break;
		case 1:
			Buff[count++]=Bytedata;
			if((count-1)==Data_len )
			{
				step++;
			}break;
		case 2:
			Buff[count++]=Bytedata;
			if(SUMCRC(Buff,Data_len)==Bytedata)
			{
				step++;
			}
			else
			{
				step=0;
			}break;
		case 3:
			if(Bytedata==0x5A)
			{
				DataAnalysis(data_ptr);
				step=0;
			}
			else
			{
				step=0;
			}
			break;
		default: break;
	}
}

extern "C"
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */

  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */
  HAL_UART_Receive_IT(&huart5, rx_buffer, 1);
  /* USER CODE END UART5_IRQn 1 */
}