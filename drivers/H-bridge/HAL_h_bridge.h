/*
 * h_bridge.h
 *
 *  Created on: [Your Date]
 *      Author: [Your Name]
 *  Description: Generic H-Bridge motor driver template for STM32 HAL.
 */

#ifndef INC_H_BRIDGE_H_
#define INC_H_BRIDGE_H_

#include "main.h" 

// 电机状态定义
typedef enum {
    MOTOR_STOP_COAST,  // 停止（自由滑行）
    MOTOR_STOP_BRAKE,  // 停止（刹车）
    MOTOR_FORWARD,     // 正转
    MOTOR_REVERSE      // 反转
} Motor_State_t;


// H桥电机控制结构体
// 用户需要为每个电机创建一个此类型的实例并填充参数
typedef struct {
    // PWM 控制 (速度)
    TIM_HandleTypeDef*  pwm_timer;        // 指向PWM定时器的句柄 (e.g., &htim3)
    uint32_t            pwm_channel;      // PWM定时器通道 (e.g., TIM_CHANNEL_1)
    uint16_t            pwm_max_duty;     // PWM最大占空比值 (等于CubeMX中设置的ARR值)

    // GPIO 控制 (方向)
    GPIO_TypeDef*       in1_port;         // IN1 引脚的GPIO端口 (e.g., IN1_GPIO_Port)
    uint16_t            in1_pin;          // IN1 引脚 (e.g., IN1_Pin)

    GPIO_TypeDef*       in2_port;         // IN2 引脚的GPIO端口 (e.g., IN2_GPIO_Port)
    uint16_t            in2_pin;          // IN2 引脚 (e.g., IN2_Pin)

} HBridge_Motor;


/**
 * @brief  初始化H桥电机驱动
 * @param  motor: 指向电机实例的指针
 * @retval None
 * @note   此函数会启动PWM信号并设置电机为滑行停止状态
 */
void HBridge_Init(HBridge_Motor* motor);

/**
 * @brief  设置电机速度和方向
 * @param  motor: 指向电机实例的指针
 * @param  speed: 速度值，范围 -100 到 100
 *                - 负值 (-1 to -100) 表示反转
 *                - 正值 (1 to 100) 表示正转
 *                - 0 表示停止 (自由滑行)
 * @retval None
 */
void HBridge_SetSpeed(HBridge_Motor* motor, int16_t speed);

/**
 * @brief  使电机停止（自由滑行）
 * @param  motor: 指向电机实例的指针
 * @retval None
 * @note   IN1=LOW, IN2=LOW。电机将因摩擦力自然停止，功耗低。
 */
void HBridge_Coast(HBridge_Motor* motor);

/**
 * @brief  使电机刹车（快速制动）
 * @param  motor: 指向电机实例的指针
 * @retval None
 * @note   IN1=HIGH, IN2=HIGH (或根据H桥芯片定义)。通过短接电机两端实现快速制动。
 */
void HBridge_Brake(HBridge_Motor* motor);


#endif /* INC_H_BRIDGE_H_ */