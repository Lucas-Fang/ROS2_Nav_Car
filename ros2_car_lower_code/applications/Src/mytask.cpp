#include "mytask.h"
#include "chassic.h"
// #include "JDB37_520_330rpm.h"



extern uint8_t rx_buffer[1];

extern "C"
{
void StartDefaultTask(void const * argument)
{
  (void)argument;
  MX_USB_DEVICE_Init();
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim7);

  chassic.Init();

  // dm_imu.myUART_DMA_init();
  // can_bus.bsp.Filter_Init(&hcan);
  // can_bus.bsp.CAN_Start(&hcan);

  // HAL_UART_Receive_IT(&huart5, rx_buffer, 1);

  xTaskCreate(
  ChassicTask,   // 任务函数
  "Chassicask",      // 任务名称（调试用）
  256,               // 堆栈大小（单位：words，不是 bytes）
  NULL,              // 参数
  osPriorityNormal,  // 优先级   tskIDLE_PRIORITY + 1
  NULL               // 可选返回任务句柄
  );

  for(;;)
  {
    vTaskDelete(NULL);
    osDelay(1);
  }
}

void LED_Breath_task(void const * argument)
{
    (void)argument;
    float t = 0;
    uint32_t duty;
  for(;;)
  {        
        t += STEP;
        if (t >= 2 * PI) t = 0;

        float brightness = (sinf(t) + 1.0f) / 2.0f;
        duty = (uint32_t)(brightness * MAX_DUTY);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
        osDelay(1);
  }
}

void Calc_Motor_rpm_task(void const * argument){
  (void)argument;
    TickType_t last = xTaskGetTickCount();
  for(;;){
    chassic.motor.Get_All_Speed_rpm();
    vTaskDelayUntil(&last, SAMPLE_PERIOD_MS);
  }
}
}


void ChassicTask(void* argument){
  (void)argument; 
  TickType_t last = xTaskGetTickCount();
  const TickType_t xFrequency = 5;  // 1ms = 1 ticks

  for(;;){
    // chassic.Set_Classic_Mode();
    // chassic.Mecanum_Cala();
    // chassic.motor.set_Motor_speed_pwm();
    vTaskDelayUntil(&last, xFrequency);
// INCLUDE_vTaskDelayUntil
  }
}