//*****************************************************************************
// Project: SAM_Blinky
// File:    main.c
// Author:  Masterkraft0r
//*****************************************************************************

#include <asf.h>

#include "init.h"

void error_blink(uint8_t error_code);
void led_statemachine(uint8_t state);

volatile bool state_changed;

void TC0_Handler()
{
  tc_get_status(TC0, 0);
  
  state_changed = true;
}

int main (void)
{
  uint32_t led_state = 0;

	sysclk_init();
	board_init();

  if(tc0ch0_init(2) == 0)
  {
    error_blink(0);
    while(true);
  }
  
  //if(pwmch0_init() != 0)
  //{
    //error_blink(1);
    //while(true);
  //}
  
	while(true)
	{
    if(state_changed)
    {
      state_changed = false;
      led_statemachine(led_state);
      led_state++;
      led_state %= 4;    
    }
  }  
}

//void simple_led_statemachine(uint8_t state)
//{
  //switch(state)
  //{
    //case
  //}
//}

void led_statemachine(uint8_t state)
{
  switch(state)
  {
    case 0:
      LED_Off(LED0_GPIO);
      LED_Off(LED1_GPIO);
      break;
    case 1:
      LED_On(LED0_GPIO);
      LED_Off(LED1_GPIO);
      break;
    case 2:
      LED_Off(LED0_GPIO);
      LED_On(LED1_GPIO);
      break;
    case 3:
      LED_On(LED0_GPIO);
      LED_On(LED1_GPIO);
      break;
  }
}

void error_blink(uint8_t error_code)
{
  uint32_t i;
  
  if(error_code > 3)
  {
    error_code = 3;
  }
  
  for(i = 0; i < 3; i++)
  {
    led_statemachine(3);
    delay_ms(200);
    led_statemachine(0);
    delay_ms(200);
  }
  led_statemachine(error_code);
  delay_ms(1000);
}