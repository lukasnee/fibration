/**
	******************************************************************************
	* @file    buttonDriver.c
	* @brief   button function set for rising edge interrupt.
	******************************************************************************
*/

#include "button.hpp"
#include "gpio.hpp"

uint8_t button_DebounceTick=0;
uint32_t button_HoldTick;

uint8_t ScanOrExecute=1; // single bit state. 1 - scan program cycle for highest priority button function. 0 - execute that button function with highest priority.

/**
  * @brief  Button initialization
  * @param  BFS: 							address of the specific button struct
  * @param  gpioPin: 					gpio pin to assign (0x0-0xF) 
  * @param  DebounceTimeInMs:	debounce time in milliseconds 
  * @retval None
  */

void Button_Init(button_t *button, uint16_t pin, button_type_t Type)
{

	button->GPIO_Pin=pin;
	button->Type = Type;
	button->Enabled=1;
	button->Priority=BTN_PRIOR_8; /*lowest priority*/
	button->IsHeld=0;
	button->WasPressed=0;
	button->Polarity = wucy_hal_PinRead(button->GPIO_Pin);

	button->Hold.Triggered=0;
	button->Hold.Duration=500;
	button->Hold.HasPassedDuration=0;
	button->Hold.Current=0;
	button->Hold.TriggerInterval=200;
	button->Hold.Prevent = 0;

}

buttonState_t Button_IsTriggered(button_t *button, button_priority_t priority)
{
	if (button->Enabled)
	{
		if (ScanOrExecute) /*scan for highest priority if scanning mode*/
		{
			if(priority > button->Priority)
				button->Priority = priority;
		}
		else /* execution mode */
		{
			if (!button_DebounceTick) /*prevents rapid triggering, debounce is the minimum gap time between triggering*/
			{
				if (Button_InputRead(button) && !button->IsHeld  && priority == button->Priority) /*button pressed - rising edge trigger (if priority matches)*/
				{
					switch (button->Type)
					{
					case BTN_TYPE_NORMAL:
						if (!button->IsHeld)
						{
							Button_StateSet(button);
							return 1;
						}
						break;

					case BTN_TYPE_DOUBLE_TRIGGER:

						button->Pressed ^= 1;
						if (button->Pressed && !button->IsHeld)
						{
							Button_StateSet(button);
							return 1;
						}
						break;
					}
				}

				if (!Button_IsPressed(button))/*button released - falling edge trigger*/
				{
					Button_ResetPriority(button);

					if (button->IsHeld)
					{
						Button_StateReset(button);
					}
				}
			}
		}
	}
	return 0;
}

void Button_TimeBaseRoutine(void)
{
    Button_PriorityRoutine();

	if(button_DebounceTick)
	{
		if(button_DebounceTick++>DEBOUNCE_MS)button_DebounceTick=0;
	}
	button_HoldTick++;
	/* ToDo: wasPressed() functionality, here to add constant checkin if
	pressed for all active buttons and if true set wasPressed bit to 1;*/
}

void Button_PriorityRoutine(void)
{
	ScanOrExecute =! ScanOrExecute; /*flip byte every execution*/
}

buttonState_t Button_IsPressed(button_t *button)
{
	switch(button->Type)
	{
	case BTN_TYPE_NORMAL:
		return Button_InputRead(button);
		break;

	case BTN_TYPE_DOUBLE_TRIGGER:
		return button->Pressed;
		break;
	}
	return 0;
}

buttonState_t Button_InputRead(button_t *button)
{
	return ((buttonState_t)button->Polarity^wucy_hal_PinRead(button->GPIO_Pin)?1:0);
}

buttonState_t Button_WasPressed(button_t *button)
{
	return button->WasPressed;
}

void Button_Enable(button_t *button)
{
	button->Enabled=1;
}
void Button_Disable(button_t *button)
{
	button->Enabled=0;
}

void Button_PreventHold(button_t *button)
{
	button->Hold.Prevent = 1;
}

void Button_StateSet(button_t *button)
{
	//ScanOrExecute=0; /*execute mode when button active*/
	button_DebounceTick=1;
	button->IsHeld=1;
}

void Button_StateReset(button_t *button)
{
	//ScanOrExecute=1; /*scan mode when button unactive*/
	button_DebounceTick=1;
	button->IsHeld=0;
	button->Hold.Prevent = 0;
	
	Button_ResetPriority(button);/*reset priority to lowest when button released*/
	
	button->Hold.HasPassedDuration=0; /*not held anymore*/
	button->Hold.Current=0;		/*reset current hold time*/
	button->Hold.Triggered=0;	/*reset post-hold trigger*/
}

void Button_ResetPriority(button_t *button)
{
	button->Priority=BTN_PRIOR_8; 
}

buttonState_t Button_IsEnabled(button_t *button)
{
	return button->Enabled;
}
    /*ToDo:  implement debounce to hold function*/
buttonState_t Button_IsHeld(button_t *button, uint16_t Duration, uint16_t	TriggerInterval, button_priority_t priority)
{	
	if(button->Enabled)
	{		
		Button_IsTriggered(button, priority);	/*refresh IsHeld and ScanOrExecute state*/
		
		if(!ScanOrExecute && priority == button->Priority && !button->Hold.Prevent)
		{
			if(button->IsHeld)	/*is held - do hold function procedure*/
			{
				button->Hold.Duration=Duration;
				button->Hold.TriggerInterval=TriggerInterval;
				
				if(!button->Hold.HasPassedDuration)	/*not held for configured time*/
				{
					if(!button->Hold.Current)button->Hold.Current=button_HoldTick;	/*start current hold time incrementation*/
					else if(button_HoldTick > button->Hold.Current + button->Hold.Duration)/*check if its held for configured duration*/
					{
						button->Hold.HasPassedDuration=1; /* start holding state if held for configured time*/
						button->Hold.Current=0;/*reset current hold time*/
					}
				}
				else/*is held for configured time - do interval triggering*/
				{
					if(!button->Hold.TriggerInterval) // if trigger interval is set to 0, trigger only once and reset hold function until button release
					{
						if(!button->Hold.Triggered) /*if not triggered already*/
						{
							button->Hold.Triggered=1; /*trigger hold*/
							return 1;
						}
						return 0;	
					}
					else /*if trigger interval more than 0, trigger pulse every interval(period)*/
					{
						if(!button->Hold.Current)button->Hold.Current=button_HoldTick;	/*start current hold time incrementation*/
						else if(button_HoldTick > button->Hold.Current + button->Hold.TriggerInterval)/*check if its held for configured duration*/
						{
							button->Hold.Current=0;
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}
