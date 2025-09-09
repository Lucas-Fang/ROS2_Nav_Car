#include "timer_user.h"
// #include "can_receive.h"
#include "chassic.h"

// int32_t pa15,pb3,pb6,pb7,pa9,pa10;
extern "C"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
   //记得注释main.c里面的回调函数
    if (htim->Instance == TIM6)
    {
    HAL_IncTick();
    }
    
    if (htim->Instance == TIM7) {
        chassic.motor.get_all_enc();
    }
}





    // can_bus.dm_imu.IMU_RequestData(&hcan, 0x01, 0x03);
//    pa15 =  __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
        // pa9 = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_2);
        // pa10 = HAL_TIM_ReadCapturedValue(&htim1,TIM_CHANNEL_3);
        // pa15 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
        // pb3 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
        // pb6 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
        // pb7 = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
