#include "key_scan.h"

// 定义一个静态的按键数组来管理所有按键
static Key_t keys[NUM_OF_KEYS];

// 读取物理按键的电平
static uint8_t Read_Key_Pin(Key_t* key)
{
    if (HAL_GPIO_ReadPin(key->port, key->pin) == KEY_PRESSED_LEVEL) {
        return 1; // 按下
    } else {
        return 0; // 未按下
    }
}

/**
 * @brief  初始化所有按键
 * @note   此函数现在使用 key_scan.h 中定义的宏进行配置
 */
void Keys_Init(void)
{
    // 初始化第一个按键 (ID: 0)
    keys[0].key_id = 0;
    keys[0].port   = KEY0_PORT; 
    keys[0].pin    = KEY0_PIN;    义
    keys[0].state  = KEY_STATE_IDLE;
    keys[0].timer  = 0;
    keys[0].event  = EVENT_NONE;
    
    // 初始化第二个按键 (ID: 1)
    keys[1].key_id = 1;
    keys[1].port   = KEY1_PORT; 
    keys[1].pin    = KEY1_PIN;    
    keys[1].state  = KEY_STATE_IDLE;
    keys[1].timer  = 0;
    keys[1].event  = EVENT_NONE;

    // 初始化第三个按键 (ID: 2)
    keys[2].key_id = 2;
    keys[2].port   = KEY2_PORT; 
    keys[2].pin    = KEY2_PIN;    
    keys[2].state  = KEY_STATE_IDLE;
    keys[2].timer  = 0;
    keys[2].event  = EVENT_NONE;
    
    // 如果在头文件中定义了更多的按键，请确保在这里也进行初始化
    // 例如，如果有 KEY3:
    #if NUM_OF_KEYS > 3
    keys[3].key_id = 3;
    keys[3].port   = KEY3_PORT;
    keys[3].pin    = KEY3_PIN;
    keys[3].state  = KEY_STATE_IDLE;
    keys[3].timer  = 0;
    keys[3].event  = EVENT_NONE;
    #endif
}

/**
 * @brief  扫描所有按键的状态机 
 */
void Keys_Scan(void)
{
    uint8_t i;
    for (i = 0; i < NUM_OF_KEYS; i++)
    {
        Key_t* key = &keys[i];

        switch (key->state)
        {
            case KEY_STATE_IDLE:
                if (Read_Key_Pin(key)) {
                    key->state = KEY_STATE_DEBOUNCE;
                    key->timer = HAL_GetTick();
                }
                break;
            
            case KEY_STATE_DEBOUNCE:
                if ((HAL_GetTick() - key->timer) > DEBOUNCE_TIME) {
                    if (Read_Key_Pin(key)) {
                        key->state = KEY_STATE_PRESSED;
                        key->timer = HAL_GetTick();
                    } else {
                        key->state = KEY_STATE_IDLE;
                    }
                }
                break;
            
            case KEY_STATE_PRESSED:
                if (!Read_Key_Pin(key)) {
                    key->state = KEY_STATE_IDLE;
                    key->event = EVENT_SINGLE_CLICK;
                } else if ((HAL_GetTick() - key->timer) > LONG_PRESS_TIME) {
                    key->state = KEY_STATE_LONG_PRESS;
                    key->event = EVENT_LONG_PRESS;
                }
                break;
            
            case KEY_STATE_LONG_PRESS:
                if (!Read_Key_Pin(key)) {
                    key->state = KEY_STATE_IDLE;
                }
                break;
            
            default:
                key->state = KEY_STATE_IDLE;
                break;
        }
    }
}

/**
 * @brief  获取指定ID按键的事件 (无需修改)
 */
KeyEvent_e Get_Key_Event(uint8_t key_id)
{
    if (key_id >= NUM_OF_KEYS) {
        return EVENT_NONE;
    }
    
    KeyEvent_e event = keys[key_id].event;
    keys[key_id].event = EVENT_NONE;
    
    return event;
}