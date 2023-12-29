/*
 * main.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Aziz
 */

#include"../LIB/STD_TYPES.h"

#include "../MCAL/PORT/PORT_interface.h"
#include "../MCAL/DIO/DIO_interface.h"
#include "../MCAL/GIE/GIE_interface.h"
#include "../MCAL/TIMER/TIMER_interface.h"

#include "../HAL/CLCD/CLCD_interface.h"

#include <util/delay.h>

void ICU_HW(void);

static volatile u16 Period_Ticks=0;
static volatile u16 ON_Ticks=0;

volatile u16 Local_u16Reading1=0;
volatile u16 Local_u16Reading2=0;
volatile u16 Local_u16Reading3=0;
void main (void)
{
	PORT_voidInit();
	CLCD_voidInit();
	_delay_ms(100);
	TIMER0_voidInt();
	ICU_u8SetCallBack(&ICU_HW);
	ICU_voidInit();
	GIE_voidEnableGlobal();
	while(1)
	{

		while((Period_Ticks==0) || (ON_Ticks==0))
		{

		}
		CLCD_voidGoToXY(0,0);
		CLCD_voidWriteString("Period=");
		ClCD_voidWriteInteger(Period_Ticks);


		CLCD_voidGoToXY(0,1);
		CLCD_voidWriteString("ON=");
		ClCD_voidWriteInteger(ON_Ticks);
	}
}
void ICU_HW(void)
{
	static volatile u8 Local_u8Counter=0;

	Local_u8Counter++;
	if(Local_u8Counter==1)
	{
		Local_u16Reading1=ICU_u16ReadInputCapture();
	}
	else if(Local_u8Counter==2)
	{
		Local_u16Reading2=ICU_u16ReadInputCapture();

		Period_Ticks=Local_u16Reading2-Local_u16Reading1;

		ICU_u8SetTrigger(FALLING_EDGE);
	}
	else if (Local_u8Counter==3)
	{
		Local_u16Reading3=ICU_u16ReadInputCapture();

		ON_Ticks=Local_u16Reading3-Local_u16Reading2;

		ICU_voidInterruptDisable();
	}
}

