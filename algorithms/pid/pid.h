/**
 * @file pid_controller.h
 * @brief 通用PID控制器模块头文件
 * @details 提供了经典位置式PID和增量式PID两种控制器的定义与函数声明。
 *          用户可以根据具体应用场景选择合适的PID算法。
 */
#ifndef PID_H
#define PID_H

//================== 用户配置区 ==================//

/**
 * @brief 控制周期，单位：秒
 * @details 该宏定义了PID计算的执行周期。
 *          **至关重要**: 此值必须与 `pid_..._calculate` 函数被调用的实际频率严格一致。
 *          例如，如果在100Hz的定时器中断中调用PID计算，则此值应为 0.01f。
 */
#define CONTROL_PERIOD_S (0.01f) 

//================== 位置式PID (经典实现) ==================//

/**
 * @brief 位置式PID控制器结构体
 * @details 存储了位置式PID控制器的所有参数和状态变量。
 */
typedef struct
{
	// --- PID参数 ---
	float Kp;             // 比例(Proportional)系数
	float Ki;             // 积分(Integral)系数
	float Kd;             // 微分(Derivative)系数

	// --- 状态变量 ---
	float integral;       // 积分项累加值
	float prev_error;     // 上一次的误差值 e(k-1)，用于计算微分项

	// --- 输出与积分限制 ---
	float output_min;     // 控制器输出的最小值
	float output_max;     // 控制器输出的最大值
	float integral_max;   // 积分项的饱和限制值，用于抗积分饱和
} PID_Positional_t;

/**
 * @brief 初始化位置式PID控制器
 * @param pid       指向要初始化的PID控制器结构体的指针
 * @param Kp        比例(Proportional)系数
 * @param Ki        积分(Integral)系数
 * @param Kd        微分(Derivative)系数
 * @param out_min   控制器输出的最小值
 * @param out_max   控制器输出的最大值
 * @param int_max   积分项的最大限制值 (用于抗积分饱和)
 */
void pid_positional_init(PID_Positional_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max, float int_max);

/**
 * @brief 计算位置式PID的输出值
 * @param pid           指向PID控制器的指针
 * @param setpoint      目标值 (设定值)
 * @param current_value 当前测量值 (反馈值)
 * @return float        经过PID计算和限幅后的控制输出量
 */
float pid_positional_calculate(PID_Positional_t* pid, float setpoint, float current_value);


//================== 增量式PID ==================//

/**
 * @brief 增量式PID控制器结构体
 * @details 存储了增量式PID控制器的所有参数和状态变量。
 */
typedef struct
{
	// --- PID参数 ---
	float Kp;             // 比例(Proportional)系数
	float Ki;             // 积分(Integral)系数
	float Kd;             // 微分(Derivative)系数
	
	// --- 状态变量 ---
	float last_error;     // 上一次的误差 e(k-1)
	float prev_error;     // 上上次的误差 e(k-2)
	float last_output;    // 上一次的控制器输出值
	
	// --- 输出限制 ---
	float output_min;     // 控制器输出的最小值
	float output_max;     // 控制器输出的最大值
} PID_Incremental_t;

/**
 * @brief 初始化增量式PID控制器
 * @param pid       指向要初始化的增量式PID控制器结构体的指针
 * @param Kp        比例(Proportional)系数
 * @param Ki        积分(Integral)系数
 * @param Kd        微分(Derivative)系数
 * @param out_min   控制器输出的最小值
 * @param out_max   控制器输出的最大值
 */
void pid_incremental_init(PID_Incremental_t* pid, float Kp, float Ki, float Kd, float out_min, float out_max);

/**
 * @brief 计算增量式PID的输出值
 * @param pid           指向增量式PID控制器的指针
 * @param setpoint      目标值 (设定值)
 * @param current_value 当前测量值 (反馈值)
 * @return float        经过PID计算和限幅后的控制输出量
 */
float pid_incremental_calculate(PID_Incremental_t* pid, float setpoint, float current_value);


#endif // PID_H