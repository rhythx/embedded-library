/*
 * h_bridge.c
 *
 *  Created on: [Your Date]
 *      Author: [Your Name]
 *  Description: Implementation of the H-Bridge motor driver.
 */

#include "h_bridge.h"
#include <stdlib.h> // For abs()

/**
 * @brief  内部函数，用于设置PWM占空比
 * @param  motor: 指向电机实例的指针
 * @param  duty: PWM占空比值 (0 to motor->pwm_max_duty)
 * @retval None
 */
static void HBridge_SetPWM(HBridge_Motor* motor, uint16_t duty)
{
    // 确保占空比不超过最大值
    if (duty > motor->pwm_max_duty) {
        duty = motor->pwm_max_duty;
    }
    
    // 使用HAL库提供的宏来设置PWM比较值（即占空比）
    __HAL_TIM_SET_COMPARE(motor->pwm_timer, motor->pwm_channel, duty);
}

/**
 * @brief  初始化H桥电机驱动
 */
void HBridge_Init(HBridge_Motor* motor)
{
    // 启动PWM定时器通道
    HAL_TIM_PWM_Start(motor->pwm_timer, motor->pwm_channel);
    
    // 初始化时，将电机设置为停止状态
    HBridge_Coast(motor);
}

/**
 * @brief  设置电机速度和方向
 */
void HBridge_SetSpeed(HBridge_Motor* motor, int16_t speed)
{
    // 限制速度在-100到100之间
    if (speed > 1000)  speed = 1000;
    if (speed < -1000) speed = -1000;
    
    if (speed > 0) // 正转
    {
        HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
        
        // 将速度 (1-100) 映射到PWM占空比 (0-max_duty)
        uint16_t duty = (uint32_t)speed * motor->pwm_max_duty / 100;
        HBridge_SetPWM(motor, duty);
    }
    else if (speed < 0) // 反转
    {
        HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
        
        // 取速度绝对值并映射到PWM占空比
        uint16_t duty = (uint32_t)abs(speed) * motor->pwm_max_duty / 100;
        HBridge_SetPWM(motor, duty);
    }
    else // 速度为0，停止（自由滑行）
    {
        HBridge_Coast(motor);
    }
}

/**
 * @brief  使电机停止（自由滑行）
 */
void HBridge_Coast(HBridge_Motor* motor)
{
    // 禁用电机驱动芯片的输入
    HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_RESET);
    
    // 将PWM占空比设为0
    HBridge_SetPWM(motor, 0);
}

/**
 * @brief  使电机刹车（快速制动）
 */
void HBridge_Brake(HBridge_Motor* motor)
{
    // 同时使能电机驱动芯片的两个输入 (查阅你的H桥芯片手册，通常是两个都高或都低)
    HAL_GPIO_WritePin(motor->in1_port, motor->in1_pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(motor->in2_port, motor->in2_pin, GPIO_PIN_SET);
    
    // 将PWM占空比设为0 (有些设计下，PWM仍然会影响刹车力度)
    HBridge_SetPWM(motor, 0);
}