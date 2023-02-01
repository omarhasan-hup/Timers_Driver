/*
 * timer0.c
 *
 *  Created on: Dec 22, 2022
 *      Author: Eng_Omar
 */

/*This module will be accuratly working when the CPU frequency is 1MHZ Only*/

/*******************************************************************************
 *                      Includes                                               *
 *******************************************************************************/
#include "timer0.h"
/*******************************************************************************
 *                      Global variables                                               *
 *******************************************************************************/
uint16 Time_Per_tick=0;
uint32 Timer_freq=0;
uint16 Number_of_Over_Flows=0;//This Variable will hold the number of over flows required
volatile uint16 Over_Flows_Counter=0;// This variable will hold the over flows in the ISR & it is volatile to avoid the compiler optimization from erasing it as it is only called in the ISR
uint8 Over_Flow_Flag=0;//this Flag will be (1) in case there will be multiple over flows else it will be (0)it state it will not complete a full over flow (only once)

uint8 test_point=0;
void(*ptr2appfunc)(void)=NULL;
/*Pointer to function which take void and return void. while this function will be edited in the app component */
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/*This function is used to update the pointer to the application function so this pointer will be called to
 *refer to the function inside the ISR of timer0 */
void Update_Ptr2appfunc(void(*ptr2func)(void))
{
	ptr2appfunc=ptr2func;
	/*Fill the pointer to application function with the sent address from the application  */
}
/*This function will be the intialization of the NORMAL mode for time0
 *It will take as an arguments the follwoing:
 *The prescaler to configure the Fcpu  */
void Timer0_Normal_Mode_Init(Prescaler_type prescaler)
{
	/*First we need to clear the I bit to disable the interrupts during the intialization*/
	SREG =SREG & (~(1<<7));

	/*Set the Bit FOC0 to activate the Normal mode or the CTC mode without generating a PWM signal */
	TCCR0=TCCR0 |(1<<FOC0);

	/*Then Clear the Bites WGM01 & WGM00 to choose the Normal mode as a timer mode*/
	TCCR0= TCCR0 & (~((1<<WGM00)|(WGM01)));

	/*Chosse the prescaler type and configure the bits CS00 & CS01 & CS02*/
	switch(prescaler)
	{

	case No_Prescaler:
		TCCR0= TCCR0 & (~(1<<CS02));
		TCCR0= TCCR0 & (~(1<<CS01));
		TCCR0=TCCR0 |(1<<CS00);
		Time_Per_tick=1; //In case of the FCPU is equal to 1MHZ
		Timer_freq=(uint16)1000000/Time_Per_tick;
		break;

	case Prescaler_8:
		TCCR0= TCCR0 & (~(1<<CS02));
		TCCR0= TCCR0 & (~(1<<CS00));
		TCCR0=TCCR0 |(1<<CS01);
		Time_Per_tick=8; //In case of the FCPU is equal to 1MHZ
		Timer_freq=1000000/Time_Per_tick;
		break;

	case Prescaler_64:
		TCCR0= TCCR0 & (~(1<<CS02));
		TCCR0=TCCR0 |(1<<CS00);
		TCCR0=TCCR0 |(1<<CS01);
		Time_Per_tick=64; //In case of the FCPU is equal to 1MHZ
		Timer_freq=1000000/Time_Per_tick;
		break;

	case Prescaler_256:
		TCCR0= TCCR0 & (~(1<<CS01));
		TCCR0= TCCR0 & (~(1<<CS00));
		TCCR0=TCCR0 |(1<<CS02);
		Time_Per_tick=256; //In case of the FCPU is equal to 1MHZ
		Timer_freq=1000000/Time_Per_tick;
		break;

	case Prescaler_1024:
		TCCR0= TCCR0 & (~(1<<CS01));
		TCCR0=TCCR0 |(1<<CS00);
		TCCR0=TCCR0 |(1<<CS02);
		Time_Per_tick=1024; //In case of the FCPU is equal to 1MHZ
		Timer_freq=1000000/Time_Per_tick;
		break;

	default:
		/*Do Nothing Because there are no prescalers left to use */
		break;
	}

	/*Clear the TCNT0 register at the intialization function*/
	TCNT0=0;

	/*Enable the over flow interrupt to enable the timer to call the ISR when called*/

	TIMSK = TIMSK |(1<<TOIE0);

	/*Set the I bit again to enable the interrupts*/
	SREG=SREG|(1<<7);
}

/*This function will be called from any module which wants to create the a time delay by timer0
 *it takes as an argument the time delay in mille seconds*/
void timer0_Normal_Mode_Delay(uint16 Delay_time)
{
	static uint32 Required_Delay_mille=0; //This Variable will store the time required in mille seconds
	static uint32 Required_Delay_micro=0;//This Variable will store the time required in micro seconds
	static uint32 Num_Ticks_required=0;//This Variable will store the number of ticks required to achieve this time delay
	static float32 Num_OverFlows=0;    // This variable will hold the number of over flows required to achieve this time delay
	uint32 Remender_Check=0;
	Required_Delay_mille=Delay_time;
	Required_Delay_micro=((uint32)Required_Delay_mille*((uint32)1000));
	Num_Ticks_required=Required_Delay_micro/Time_Per_tick;
	Num_OverFlows=(float32)Num_Ticks_required/(uint16)256;
	/*Testing: no_presaler and delay is 1000ms Num_OverFlow 3906*/
	/* the devision result (Num_Overflows) will be float and one of the arguments (Num_Ticks_required) also will be a float so we could
	 * get a float number in the result*/
	if(Num_OverFlows < 1 )
	{
		/*If the number of over flows is less than one then timer0 will not finish even one complete over flow and will start from
		 *  specific number to achieve the delay time required. How to do So?
		 *
		 *  1)For Example:Num_OverFlows=0.400 ==> then the timer0 will achieve the required time in only 0.400*256 tick = 102 ticks
		 *  2)Then timer0 will start counting from the number (256-102)= 154  ==> TCNT0=154
		 */
		static uint8 Num_Ticks,Start_Count_From=0;
		Num_Ticks=((float32)Num_OverFlows)*((uint16) 256);// Step one as explained above
		Start_Count_From=((uint16)256) - Num_Ticks;      //  Step two as explained above
		TCNT0=Start_Count_From;
		test_point=2;
	}
	else
	{
		/*If the Num_OverFlows is higher than 1 then it means that the timer0 will over flow multiple times either
		 *Num_OverFlows = (1*n) || Num_OverFlows = (1.y*n)
		 *Then we want to differentiate between the (1*n) || (1.y *n) ==> we want a variable to check the remender (Num_Ticks_required % 256) */

		Remender_Check=Num_Ticks_required %((uint16)256);
		if(Remender_Check != 0)
		{
			/*If there is a reminder then we are in the case (1.y)*n of over flows then we must get the 0.y part as the case above
			 *then start with it then keep over flowing till to reach (n)*/
			float32 Reminder_OverFlow=0;// this variable will store the reminder of the devision
			static uint8 Num_Ticks,Start_Count_From=0;
			Reminder_OverFlow=(((float32)Num_OverFlows)-((uint32)Num_OverFlows)*1000); // This will get the part of un completed over flows
			Reminder_OverFlow=(Reminder_OverFlow/(float32)1000);
			Num_Ticks=((float32)Reminder_OverFlow)*((uint16) 256);// Step one as explained above
			Start_Count_From=((uint16)256) - Num_Ticks;      //  Step two as explained above
			TCNT0=Start_Count_From;
			Number_of_Over_Flows=(uint32)Num_OverFlows;
			Over_Flow_Flag=1;//set the over flow flag because there will be several overflows.

			test_point=11;

		}
		else
		{
			/*If there was no reminder and the number of over flow is higher than 1 then the number of over flows is the same number */
			Number_of_Over_Flows=((uint32)Num_OverFlows);
			Over_Flow_Flag=1;//set the over flow flag because there will be several overflows.
			test_point=1;

		}


	}

}



ISR(TIMER0_OVF_vect)
{
	/*This is where what will happen when each over flow happen
	 *In case: of the uncompleted over flow
	 *In which will start from a specific TCNT0 till the 256
	 *
	 *In Case: Of completed several over flows with specific TCNT0
	 *In which there will be several over flows and one uncompleted over flow at the begging
	 *
	 *In Case: Of completed several over flows with TCNT0 =0
	 *In which the ISR will keep incresing the over flow counter till it reach the required number */
	if(Over_Flow_Flag==1)
	{
		/*when the Over_Flow_Flag ==1 this means you will make several over follows so each ISR increment the Over_Flows_Counter
		 *Until it reach the required number then call the function of the application to execute what is inside it*/

		Over_Flows_Counter++;
		if(Over_Flows_Counter==Number_of_Over_Flows)
		{
			/*When the number of the over flow counter reaches the number of over follow required then call the pointer to the app function*/
			if(ptr2appfunc!=NULL)
			{
				/*Check if the pointer to application function not equal to NULL which means that the application
				 *has filled it with the required function to do*/
				ptr2appfunc();
				Over_Flows_Counter=0;//after finishing what is inside the pointer to app function just but the over flow count to zero again
			}
			else
			{
				/*Do nothing because there are no address in the pointer to the application function*/
			}
		}
	}
	else
	{
		/*Which means that the Over_Flow_Flag ==0 which means there will be no over flow so once the ISR fires go and call the pointer to the app function */
		if(ptr2appfunc!=NULL)
		{
			/*Check if the pointer to application function not equal to NULL which means that the application
			 *has filled it with the required function to do*/
			ptr2appfunc();
			Over_Flows_Counter=0;//after finishing what is inside the pointer to app function just but the over flow count to zero again
		}
		else
		{
			/*Do nothing because there are no address in the pointer to the application function*/
		}
	}

}

