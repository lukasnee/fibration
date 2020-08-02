#ifndef _BUTTON_H
#define _BUTTON_H
/**
	******************************************************************************
	* @file    button.h
	* @brief   button function set header file.
	******************************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define DEBOUNCE_MS 20

typedef uint8_t buttonState_t;

typedef enum{

	BTN_PRIOR_8,
	BTN_PRIOR_7,
	BTN_PRIOR_6,
	BTN_PRIOR_5,
	BTN_PRIOR_4,
	BTN_PRIOR_3,
	BTN_PRIOR_2,
	BTN_PRIOR_1,
	
} button_priority_t;

typedef enum{
	BTN_TYPE_NORMAL,
	BTN_TYPE_DOUBLE_TRIGGER
}button_type_t;

typedef struct{

	uint8_t 	Triggered:1; //trigger after configured hold duration
	uint8_t 	HasPassedDuration:1;
	uint8_t		Prevent:1;
	uint16_t 	Duration;
	uint32_t 	Current;
	uint16_t	TriggerInterval;

	
}button_hold_t;

typedef struct{ 
	
	button_hold_t Hold;

	uint16_t GPIO_Pin;
	button_type_t Type;

	uint8_t Pressed:		1;
	uint8_t Enabled:		1;
	uint8_t	IsHeld:			1;
	uint8_t	Polarity:		1;
	uint8_t	WasPressed:		1;

	button_priority_t Priority:	4;
	
} button_t;

/*!!! needs to be executed periodically every 1 ms !!!*/
void Button_TimeBaseRoutine(void);

///*!!! button priority check - execute at the end end of every program cicle of user interface !!! */
//void Button_PriorityRoutine(void);

/*Setup functions*/
void Button_Init(button_t *button, uint16_t pin, button_type_t Type);
void Button_Enable(button_t *button);
void Button_Disable(button_t *button);

void Button_PreventHold(button_t *button);
void Button_StateSet(button_t *button);
void Button_StateReset(button_t *button);
void Button_ResetPriority(button_t *button);
void Button_PriorityRoutine(void);

/*Status functions*/
buttonState_t Button_IsTriggered(button_t *button, button_priority_t button_priority);
buttonState_t Button_IsPressed(button_t *button);
buttonState_t Button_InputRead(button_t *button);
buttonState_t Button_WasPressed(button_t *button);
buttonState_t Button_IsEnabled(button_t *button);
buttonState_t Button_IsHeld(button_t *button, uint16_t Duration, uint16_t	TriggerInterval, button_priority_t button_priority);


#ifdef __cplusplus
}
#endif

#endif
