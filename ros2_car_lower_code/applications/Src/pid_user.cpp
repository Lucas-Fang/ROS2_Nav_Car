#include "pid_user.h"
#include "DM_imu_485.h"
#include "JDB37_520_330rpm.h"
#include "chassic.h"
#include "pid.h"
#include "struct_typedef.h"

fp32 pid_v[3] = {0,0,0};

void JDB37_520::MOTOR_PID::Motor_pid_Init(){
    pid.PID_Init(&motor_pid_handel, PID_POSITION,pid_v, 
             7000,1000, 10);
}

fp32 JDB37_520::MOTOR_PID::Velocity_Realize(fp32 set_speed){
    pid.PID_Calc(&motor_pid_handel, parent->motor_measure.rpm_filtered, set_speed);
    return motor_pid_handel.out;
}

fp32 CHASSIC::Yaw_pid_realize(fp32 set_yaw){
    pid.PID_Calc(&pid_struct, dm_imu.imu.yaw, set_yaw);
    return pid_struct.out;
}