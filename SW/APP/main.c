/*
 * main.c
 *
 *  Created on: Dec 16, 2022
 *      Author: Eng_Omar
 */


/*includes*/
#include <avr/io.h>

#include "../HAL/Timer_driver/timer0.h"
/*******************************************************/
/*Constants*/

/*******************************************************/
/*global variables*/


/*******************************************************/

/*functions prototypes*/
void timer0_func(void);
/*******************************************************/
int main(void)
{
	/*call the function which will init the timer0 module with frequency of (Fcpu/8)*/
	Timer0_Normal_Mode_Init(No_Prescaler);
	DDRD=DDRD|(1<<PD0);//make this pin as output
	PORTD=PORTD &(~(1<<PD0));//make the first output of this pin as low
	Update_Ptr2appfunc(timer0_func);//Update the pointer to app function with the written function address


	while(1)
	{
		timer0_Normal_Mode_Delay(300);//toggle the led each 500 mille seconds
	}
	return 0;

}


void timer0_func(void)
{
	PORTD=PORTD^(1<<PD0);
	/*Just toggle the led each call*/
}



/*******************************************************/
/*Functions body*/

