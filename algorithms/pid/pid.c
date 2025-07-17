/**
 * @file pid_controller.c
 * @brief 通用PID控制器模块实现文件
 */
#include "pid.h"

//================== 位置式PID (经典实现) ==================//

/**
 * @brief 初始化位置式PID控制器
 */
void pid_positional_init(PID_Positional_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max, float int_max)
{
    // 设置PID三参数
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    // 清零状态变量，为首次计算做准备
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;

    // 设置输出和积分的限幅
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->integral_max = int_max;
}

/**
 * @brief 执行一次经典位置式PID计算
 * @details 公式: Output(k) = Kp*e(k) + Ki*∑e(j) + Kd*[e(k)-e(k-1)]
 */
float pid_positional_calculate(PID_Positional_t* pid, float setpoint, float current_value)
{
    // 1. 计算当前误差 e(k)
    float error = setpoint - current_value;

    // 2. 计算积分项，并乘以控制周期T，得到累加值
    // 公式: integral += error * T
    pid->integral += error * CONTROL_PERIOD_S;
    
    // 3. 对积分项进行抗饱和处理 (Integral Anti-Windup)
    // 防止因长时间无法达到目标值导致积分项过度累积，从而引起系统严重超调
    if (pid->integral > pid->integral_max) {
        pid->integral = pid->integral_max;
    } else if (pid->integral < -pid->integral_max) {
        pid->integral = -pid->integral_max;
    }

    // 4. 计算微分项
    // 公式: d_term = Kd * (e(k) - e(k-1)) / T
    float d_term = pid->Kd * (error - pid->prev_error) / CONTROL_PERIOD_S;
    
    // 5. 计算PID的三个分量输出
    float p_term = pid->Kp * error;
    float i_term = pid->Ki * pid->integral;

    // 6. 计算总输出
    float output = p_term + i_term + d_term;

    // 7. 对总输出进行限幅，确保输出在物理允许范围内
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }

    // 8. 保存本次误差，作为下一次计算的 "上一次误差"
    pid->prev_error = error;

    return output;
}


//================== 增量式PID ==================//

/**
 * @brief 初始化增量式PID控制器
 */
void pid_incremental_init(PID_Incremental_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max)
{
    // 设置PID三参数
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    // 清零所有状态变量
    pid->last_error = 0.0f;
    pid->prev_error = 0.0f;
    pid->last_output = 0.0f;

    // 设置输出限幅
    pid->output_min = out_min;
    pid->output_max = out_max;
}

/**
 * @brief 执行一次增量式PID计算
 * @details 公式: ΔOutput(k) = Kp*[e(k)-e(k-1)] + Ki*e(k)*T + Kd*[e(k)-2e(k-1)+e(k-2)]/T
 *          Output(k) = Output(k-1) + ΔOutput(k)
 */
float pid_incremental_calculate(PID_Incremental_t* pid, float setpoint, float current_value)
{
    // 1. 计算当前误差 e(k)
    float error = setpoint - current_value;

    // 2. 计算比例、积分、微分项的增量
    float p_term_inc = pid->Kp * (error - pid->last_error);
    float i_term_inc = pid->Ki * error * CONTROL_PERIOD_S;
    float d_term_inc = pid->Kd * (error - 2.0f * pid->last_error + pid->prev_error) / CONTROL_PERIOD_S;

    // 3. 计算总增量 ΔOutput(k)
    float delta_output = p_term_inc + i_term_inc + d_term_inc;

    // 4. 计算本次输出
    // 公式: Output(k) = Output(k-1) + ΔOutput(k)
    float output = pid->last_output + delta_output;

    // 5. 对总输出进行限幅
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }

    // 6. 更新状态变量，为下一次计算做准备
    pid->prev_error = pid->last_error;  // 更新上上次误差
    pid->last_error = error;            // 更新上一次误差
    
    // 7. 更新上一次的输出值
    // **注意**: 使用限幅后的 output 更新 last_output，这是增量式PID自带的抗饱和机制。
    // 当输出达到饱和时，last_output 不会无限制增长，从而避免了积分饱和问题。
    pid->last_output = output;

    return output;
}