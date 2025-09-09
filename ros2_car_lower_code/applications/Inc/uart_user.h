#ifndef __UART_USER_H_
#define __UART_USER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "struct_typedef.h"
#include "usart.h"
#define DATA_LEN   7
struct remote_data
{
	int16_t ch[4];      
};

void DataAnalysis(uint8_t *Data);
uint8_t SUMCRC(uint8_t *Message,uint16_t DataLen);
void Receive_data(uint8_t Bytedata,uint8_t Data_len);
void BlueTooth_Send_Data(UART_HandleTypeDef *huart);


extern remote_data receive_data;
#ifdef __cplusplus
}
#endif
	
#endif