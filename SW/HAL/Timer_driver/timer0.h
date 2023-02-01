/*
 * timer0.h
 *
 *  Created on: Jan 2, 2023
 *      Author: Eng_Omar
 */
/*This module will be accuratly working when the CPU frequency is 1MHZ Only*/
#ifndef HAL_TIMER_DRIVER_TIMER0_H_
#define HAL_TIMER_DRIVER_TIMER0_H_
/*******************************************************************************
 *                      Includes                                               *
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#include "avr/interrupt.h"
/*******************************************************************************/

/*******************************************************************************
 *                      Definations & Enumurtions &static types                              *
 *******************************************************************************/
typedef enum
{
	No_Prescaler,
	Prescaler_8,
	Prescaler_64,
	Prescaler_256,
	Prescaler_1024
}Prescaler_type;

/*******************************************************************************
 *                      Functions prototypes                                               *
 *******************************************************************************/
/*This function is used to update the pointer to the application function so this pointer will be called to
 *refer to the function inside the ISR of timer0 */
void Update_Ptr2appfunc(void(*ptr2func)(void));
/*This function will be the intialization of the NORMAL mode for time0
 *It will take as an arguments the follwoing:
 *The prescaler to configure the Fcpu  */
void Timer0_Normal_Mode_Init(Prescaler_type prescaler);
/*This function will be called from any module which wants to create the a time delay by timer0
 *it takes as an argument the time delay in mille seconds*/
void timer0_Normal_Mode_Delay(uint16 Delay_time);


/*******************************************************************************/


#endif /* HAL_TIMER_DRIVER_TIMER0_H_ */
