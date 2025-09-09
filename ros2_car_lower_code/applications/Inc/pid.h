#ifndef __PID_H_
#define __PID_H_

#include "struct_typedef.h"
#ifdef __cplusplus
extern "C" 
{
#endif

#define LimitMax(input, max)   \
{                          	   \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
}


typedef enum
{
    PID_POSITION = 0,
    PID_DELTA
}PID_MODE;

typedef struct
{
    uint8_t mode;
    //PID 三参
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出限幅
    fp32 max_iout; //最大积分输出限幅

    fp32 set;
    fp32 fdb;

    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�
    fp32 error[3]; //����� 0���� 1��һ�� 2���ϴ�

	fp32 deadband_threshold;

} pid_type_def;

class PID_Controller
{
	public:
	
		// class CORE
		// {
			// public:
				void PID_Init(pid_type_def *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout,fp32 deadband_threshold);
				fp32 PID_Calc(pid_type_def *pid, fp32 ref, fp32 set);
				void PID_Clear(pid_type_def *pid);
		// }core;
		
		// class MOTOR
		// {
		// 	public:
		// 		fp32 Velocity_Realize(fp32 set_speed);
		// 		fp32 CAN1_Position_Realize(fp32 set_pos,int i);
		// 		fp32 CAN1_VP_Dual_Loop_Realize(fp32 set_pos,int i,int mode);

		// 		fp32 CAN1_Absolute_Pos_Realize(fp32 set_pos , int i);
		// 	private:
		// 		pid_type_def motor_pid; 
		// 		PID_Controller::CORE pid_core;
		// }can_motor;
		
		class SENSORS
		{
			public:
				fp32 Yaw_Realize(fp32 set_yaw);
			
				fp32 Pos_X_Realize(fp32 set_pos_x);
				fp32 Pos_Y_Realize(fp32 set_pos_y);
		}sensors;

	
};
	
	

#ifdef __cplusplus
}
#endif

#endif
