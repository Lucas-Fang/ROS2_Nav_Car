#ifndef __JDB37_520_330RPM_H_
#define __JDB37_520_330RPM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "tim.h"
#include "main.h"
#include "struct_typedef.h"
#include "math.h"
#include "pid.h"


#define ENCODER_LINES       11.0f  //线数
#define ENCODER_MULTIPLIER  4.0f   //倍频
#define MOTORRATIO          30.0f //减速比
#define SAMPLE_PERIOD_MS    10.0f   // 采样频率10ms
#define ALPHA               0.2f     // 一阶滤波系数

typedef struct {
    int16_t CNT_value;
    int64_t CNT_total_value;
    int64_t last_total_value;
    fp32 speed_rpm;
    fp64 rpm_filtered;
}Motor_Measure;


typedef struct {
    GPIO_TypeDef *IN1_Port;
    uint16_t      IN1_Pin;
    GPIO_TypeDef *IN2_Port;
    uint16_t      IN2_Pin;

    TIM_HandleTypeDef *pwmTim;
    uint32_t      pwmChannel;

    // TIM_TypeDef *encTim;
    TIM_HandleTypeDef *encTim;
    // uint16_t     countsPerRev;   // 编码器每转脉冲数

} MotorConfig;

class JDB37_520{
    private:
        MotorConfig config;
        Motor_Measure motor_measure;

    public:
        JDB37_520(const MotorConfig& cfg) : config(cfg),motor_pid(this) {
        }
        void Motor_Config_Init();
        void setSpeed(int16_t speed_pwm);
        void stop();
        void get_Motor_Encoder();
        void Calc_Motor_rpm();

    class MOTOR_PID{
        public:
            MOTOR_PID(JDB37_520* parent) : parent(parent) {}
            void Motor_pid_Init();
            fp32 Velocity_Realize(fp32 set_speed);
        private:
            JDB37_520* parent; // 保存外部类指针
            pid_type_def motor_pid_handel; 
            PID_Controller pid;//lei
    }motor_pid;
};

extern MotorConfig motorConfigs[4];

#ifdef __cplusplus
}
#endif
#endif