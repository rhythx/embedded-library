#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#include "main.h" 
#include <stdint.h>

//-------------------- 用户配置区 START --------------------

// 1. 定义按键数量
#define NUM_OF_KEYS         3

// 2. 定义按键事件的判定时间 (单位: ms)
#define DEBOUNCE_TIME       20   // 消抖时间
#define LONG_PRESS_TIME     1000 // 长按判定时间

// 3. 定义按键的物理电平
// 如果按键按下时是低电平 (GND)，则为 0
// 如果按键按下时是高电平 (VCC)，则为 1
#define KEY_PRESSED_LEVEL   0

// 4. 定义每个按键的GPIO引脚
// 格式: #define KEYx_PORT   GPIOx
//       #define KEYx_PIN    GPIO_PIN_x

// 按键 0 (ID: 0)
#define KEY0_PORT           GPIOC
#define KEY0_PIN            GPIO_PIN_13

// 按键 1 (ID: 1)
#define KEY1_PORT           GPIOA
#define KEY1_PIN            GPIO_PIN_0

// 按键 2 (ID: 2)
#define KEY2_PORT           GPIOB
#define KEY2_PIN            GPIO_PIN_5

// 如果有更多按键，在这里继续添加...
// #define KEY3_PORT        ...
// #define KEY3_PIN         ...

//-------------------- 用户配置区 END --------------------

// 按键状态枚举 
typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_DEBOUNCE,
    KEY_STATE_PRESSED,
    KEY_STATE_LONG_PRESS,
    KEY_STATE_RELEASE_DEBOUNCE
} KeyState_e;

// 按键事件枚举 
typedef enum {
    EVENT_NONE,
    EVENT_SINGLE_CLICK,
    EVENT_LONG_PRESS
} KeyEvent_e;

// 按键信息结构体
typedef struct {
    uint8_t         key_id;
    GPIO_TypeDef*   port;
    uint16_t        pin;
    KeyState_e      state;
    uint32_t        timer;
    KeyEvent_e      event;
} Key_t;


/**
 * @brief  初始化所有按键
 * @param  None
 * @retval None
 */
void Keys_Init(void);

/**
 * @brief  扫描所有按键的状态
 * @param  None
 * @retval None
 */
void Keys_Scan(void);

/**
 * @brief  获取指定ID按键的事件
 * @param  key_id: 要获取事件的按键ID
 * @retval KeyEvent_e: 按键的事件
 */
KeyEvent_e Get_Key_Event(uint8_t key_id);


#endif /* __KEY_SCAN_H */