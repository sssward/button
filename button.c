#include "button.h"

/**
 * @param button 传入的按键控制块
 * @param idleLevel 按键没有按下时的空闲电平
 * @param doublePress_Threshold 第一次按键单击完成后停止对第二次按键按下的等待转而直接判定为单击的最大时间阈值，单位为10ms，推荐20
 * @param longPress_Threshold   第一次按下不松开一直到判定为长按的阈值，单位10ms，推荐100,
 *                             
 * @param getButtonVal 传入自己实现的按键状态检测回调函数
 */
void buttonInit(button_t *button, idleLevel_t idleLevel, uint16_t doublePress_Threshold, uint16_t longPress_Threshold, getButtonVal_t getButtonVal)
{
    button->val = idleLevel;
    button->valPre = idleLevel;
    button->ticks = 0;    
    button->idleLevel = idleLevel;
    button->doublePress_Threshold = doublePress_Threshold;
    button->longPress_Threshold = longPress_Threshold;
    button->state = IDLE;    
    button->getButtonVal = getButtonVal;
    button->eventHandler[SINGLE_CLICK] = NULL;
    button->eventHandler[DOUBLE_CLICK] = NULL;
    button->eventHandler[LONG_PRESS]   = NULL;
}

/**
 * @brief 用于注册按键事件，同时传入相对应事件触发时的回调函数
 * 
 * @param button 传入的按键控制块
 * @param buttonEvent 想要注册的事件类型（单机，双击，长按）
 * @param eventHandler 传入触发对应事件时的回调函数
 * 
 * @note 没有注册过的事件，按键即使做了相对应的动作也不会有响应
 */
void buttonLink(button_t *button, buttonEvent_t buttonEvent, eventHandler_t eventHandler)
{
    button->eventHandler[buttonEvent] = eventHandler;
}

/**
 * @brief 按键状态机扫描函数，需要以固定10ms周期扫描
 * 
 * @param button 传入的按键控制块
 */
void buttonScan(button_t *button)
{
    button->val = button->getButtonVal();
    switch (button->state)
    {
        case IDLE:
        {
            if((button->val != button->idleLevel) && (button->valPre == button->idleLevel))
            {
                button->state = FIRST_PRESS;            
            }
            break;
        }
        case FIRST_PRESS:
        {
            if(button->val == button->valPre)
            {
                button->ticks++;
                if(button->ticks >= button->longPress_Threshold)
                {
                    button->ticks = 0;
                    button->state = IDLE;
                    if(button->eventHandler[LONG_PRESS] != NULL)
                    {
                        button->eventHandler[LONG_PRESS]();
                    }    
                }               
            }
            else
            {
                button->ticks = 0;
                button->state = FIRST_RELEASE;
            }
            break;
        }
        case FIRST_RELEASE:
        {
            if(button->val == button->valPre)
            {
                button->ticks++;
                if(button->ticks >= button->doublePress_Threshold)
                {
                    button->ticks = 0;
                    button->state = IDLE;
                    if(button->eventHandler[SINGLE_CLICK] != NULL)
                    {
                        button->eventHandler[SINGLE_CLICK]();
                    }
                }
            }
            else
            {
                button->ticks = 0;
                button->state = SECOND_PRESS;
            }
            break;
        }
        case SECOND_PRESS:
        {
            if(button->val != button->valPre)
            {
                button->state = IDLE;
                if(button->eventHandler[DOUBLE_CLICK] != NULL)
                {
                    button->eventHandler[DOUBLE_CLICK]();
                }
            }
            break;
        }
        default:
            break;
    }
    button->valPre = button->val;
}
