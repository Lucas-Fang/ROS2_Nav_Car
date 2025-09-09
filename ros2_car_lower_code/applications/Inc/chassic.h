#ifndef __CHASSIC_H_
#define __CHASSIC_H_

#include "pid.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include "tim.h"
#include "main.h"
#include "struct_typedef.h"
#include "math.h"
#include "usart.h"
#include "usb_device.h"
#include "DM_imu_485.h"
#include "JDB37_520_330rpm.h"
#include "uart_user.h"


#define MANUAL_MODE 0X01 //手动模式
#define ROS2_MODE   0X02 //ros2导航模式

struct MOVE_PARAM{
    int32_t v_x;
    int32_t v_y;
    int32_t v_omega;
};
class CHASSIC{
    private:
        MOVE_PARAM move_param ;
        int16_t target_speed_pwm[4];
        pid_type_def pid_struct;
        PID_Controller pid;
    public:
        void Set_Classic_Mode();
        void Mecanum_Cala();
        void Init();

        fp32 Yaw_pid_realize(fp32 set_yaw);

    class MOTOR{
        public:
            void set_Motor_speed_pwm();
            void Motor_Config_Init();
            void get_all_enc();
            void Get_All_Speed_rpm();
            void Set_Motor_rpm(int16_t wheel1,int16_t wheel2,int16_t wheel3,int16_t wheel4);

        private:
            //创建数组类四个电机对象
            JDB37_520 motors[4] = {
            JDB37_520(motorConfigs[0]),
            JDB37_520(motorConfigs[1]),
            JDB37_520(motorConfigs[2]),
            JDB37_520(motorConfigs[3])
            };
    }motor;
};
extern CHASSIC chassic;

#ifdef __cplusplus
}
#endif
#endif