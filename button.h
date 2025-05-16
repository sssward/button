#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef enum {
    IDLE_LEVEL_LOW  = 0,
    IDLE_LEVEL_HIGH = 1,
}idleLevel_t;   

typedef enum {
    IDLE,
    FIRST_PRESS,
    FIRST_RELEASE,
	SECOND_PRESS,
}buttonState_t;

typedef enum {
    SINGLE_CLICK = 0,
    DOUBLE_CLICK = 1,
    LONG_PRESS   = 2,
}buttonEvent_t;

typedef void (*eventHandler_t)(void);
typedef uint8_t (*getButtonVal_t)(void);

typedef struct Button {
    uint8_t val;
    uint8_t valPre;
    uint16_t ticks; 
    idleLevel_t idleLevel;
    uint16_t doublePress_Threshold;
    uint16_t longPress_Threshold; 
    buttonState_t state;
    getButtonVal_t getButtonVal;
    eventHandler_t eventHandler[3];
}button_t;

void buttonInit(button_t *button, idleLevel_t idleLevel, uint16_t doublePress_Threshold, uint16_t longPress_Threshold, getButtonVal_t getButtonVal);
void buttonLink(button_t *button, buttonEvent_t buttonEvent, eventHandler_t eventHandler);
void buttonScan(button_t *button);

#endif //BUTTON_H
