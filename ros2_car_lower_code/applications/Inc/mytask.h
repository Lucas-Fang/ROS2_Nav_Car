#ifndef __MYTASK_H_
#define __MYTASK_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "tim.h"
#include "main.h"
#include "struct_typedef.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "math.h"
#include "usart.h"
#include "usb_device.h"
#include "DM_imu_485.h"
#include "chassic.h"

#define PI 3.1415926f
#define MAX_DUTY 1000  // TIMx->ARR = 1000时，占空比最大值
#define STEP 0.003f     // 每次的 t 增量，影响快慢（呼吸速度）

void ChassicTask(void * argument);

#ifdef __cplusplus
}
#endif
	
#endif