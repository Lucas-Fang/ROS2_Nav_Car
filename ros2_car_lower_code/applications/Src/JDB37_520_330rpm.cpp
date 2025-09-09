#include "JDB37_520_330rpm.h"

//创造电机配置表
MotorConfig motorConfigs[4] = {
{GPIOC, GPIO_PIN_2, GPIOC, GPIO_PIN_3, 
    &htim5, TIM_CHANNEL_2, &htim3}, // 电机1
{GPIOB, GPIO_PIN_15, GPIOB, GPIO_PIN_14, 
    &htim5, TIM_CHANNEL_4, &htim8}, // 电机2
{GPIOB, GPIO_PIN_13,  GPIOB, GPIO_PIN_12,  
    &htim5, TIM_CHANNEL_3, &htim4},  // 电机3
{GPIOC, GPIO_PIN_0,  GPIOC, GPIO_PIN_1,  
    &htim5, TIM_CHANNEL_1, &htim2} // 电机4
};


void JDB37_520::Motor_Config_Init(){
    HAL_TIM_PWM_Start(config.pwmTim, config.pwmChannel); 
    HAL_TIM_Encoder_Start(config.encTim, TIM_CHANNEL_1 | TIM_CHANNEL_2 );
    motor_pid.Motor_pid_Init();
}

void JDB37_520::setSpeed(int16_t speed_pwm) {
    if (speed_pwm >= 0) {
        HAL_GPIO_WritePin(config.IN1_Port, config.IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(config.IN2_Port, config.IN2_Pin, GPIO_PIN_SET);
    } else {
        speed_pwm = -speed_pwm;
        HAL_GPIO_WritePin(config.IN1_Port, config.IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(config.IN2_Port, config.IN2_Pin, GPIO_PIN_RESET);
    }
    __HAL_TIM_SET_COMPARE(config.pwmTim, config.pwmChannel, speed_pwm + 500);
}

void JDB37_520::stop() {
    HAL_GPIO_WritePin(config.IN1_Port, config.IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(config.IN2_Port, config.IN2_Pin, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(config.pwmTim, config.pwmChannel, 0);
}

void JDB37_520::get_Motor_Encoder(){
    // motor_measure.CNT_value = (short)config.encTim -> CNT;   
    // motor_measure.CNT_total_value += motor_measure.CNT_value;
    // config.encTim -> CNT=0;
    motor_measure.CNT_value = (short)__HAL_TIM_GetCounter(config.encTim);//
    motor_measure.CNT_total_value += motor_measure.CNT_value;
    __HAL_TIM_SetCounter(config.encTim,0);
}
//一圈1325  减速比1:30 线数11 倍频4
void JDB37_520::Calc_Motor_rpm(){
    int16_t delta_enc_calue;
    fp32 rpm;
    delta_enc_calue = motor_measure.CNT_total_value - motor_measure.last_total_value;
    motor_measure.last_total_value = motor_measure.CNT_total_value;

    rpm = (delta_enc_calue/(ENCODER_LINES*ENCODER_MULTIPLIER*MOTORRATIO))*
                            ((1000*60)/SAMPLE_PERIOD_MS);
    motor_measure.speed_rpm = rpm;

    motor_measure.rpm_filtered = ALPHA*rpm + (1-ALPHA)*motor_measure.rpm_filtered;
}