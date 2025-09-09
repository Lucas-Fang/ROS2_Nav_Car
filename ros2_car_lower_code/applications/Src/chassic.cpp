#include "chassic.h"
#include "pid.h"
/*
       电机编号
        1   2
        3   4
*/
CHASSIC chassic;
uint8_t robot_mode;
fp32 yaw_pid[3] = {0,0,0};

void CHASSIC::Init(){
    motor.Motor_Config_Init();
    pid.PID_Init(&pid_struct, PID_POSITION ,yaw_pid, 
            4000, 1000, 0.1);
}

void CHASSIC::Set_Classic_Mode(){
    switch (robot_mode) {
        case MANUAL_MODE:
            move_param.v_x = receive_data.ch[0]*20;
            move_param.v_y = receive_data.ch[1]*20;
            move_param.v_omega = receive_data.ch[2]*10;
            // move_param.v_omega = Yaw_pid_realize(receive_data.ch[2]);
        break;

        case ROS2_MODE:
            move_param.v_x = 0;
            move_param.v_y = 0;
            move_param.v_omega = 0;
        break;
    }
}

void CHASSIC::Mecanum_Cala(){
    int16_t rpm[4];
    rpm[0] = move_param.v_y - move_param.v_x + move_param.v_omega;
	rpm[1] = move_param.v_y + move_param.v_x - move_param.v_omega;
	rpm[2] = move_param.v_y - move_param.v_x - move_param.v_omega;
	rpm[3] = move_param.v_y + move_param.v_x + move_param.v_omega;

    motor.Set_Motor_rpm(rpm[0], rpm[1], rpm[2], rpm[3]);
    // target_speed_pwm[0] = move_param.v_y - move_param.v_x + move_param.v_omega;
	// target_speed_pwm[1] = move_param.v_y + move_param.v_x - move_param.v_omega;
	// target_speed_pwm[2] = move_param.v_y - move_param.v_x - move_param.v_omega;
	// target_speed_pwm[3] = move_param.v_y + move_param.v_x + move_param.v_omega;
}

/*------------------------底盘下电机批量控制函数-------------------------------*/
void CHASSIC::MOTOR::Motor_Config_Init(){
    for (auto &m : motors) {
        m.Motor_Config_Init();
    } 
}
void CHASSIC::MOTOR::Get_All_Speed_rpm(){
   for (auto &m : motors) {
        m.Calc_Motor_rpm();
    }
}

void CHASSIC::MOTOR::set_Motor_speed_pwm(){
    uint8_t i =0;
    for (auto &m : motors) {
        m.setSpeed(chassic.target_speed_pwm[i]);
    i++;
    }   
    // motors[0].setSpeed(chassic.target_speed_pwm[0]);
    // motors[1].setSpeed(chassic.target_speed_pwm[1]);
    // motors[2].setSpeed(chassic.target_speed_pwm[2]);
    // motors[3].setSpeed(chassic.target_speed_pwm[3]);
}
void CHASSIC::MOTOR::get_all_enc(){
        for (auto &m : motors) {
        m.get_Motor_Encoder();
    } 
}
void CHASSIC::MOTOR::Set_Motor_rpm(int16_t wheel1,int16_t wheel2,int16_t wheel3,int16_t wheel4){
    chassic.target_speed_pwm[0] = motors[0].motor_pid.Velocity_Realize(wheel1);
    chassic.target_speed_pwm[1] = motors[1].motor_pid.Velocity_Realize(wheel2);
    chassic.target_speed_pwm[2] = motors[2].motor_pid.Velocity_Realize(wheel3);
    chassic.target_speed_pwm[3] = motors[3].motor_pid.Velocity_Realize(wheel4);

    // set_Motor_speed_pwm();
}





 











