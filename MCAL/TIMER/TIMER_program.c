/*****************************************************************/
/*****************************************************************/
/***************		Author: Aziz Tarek		******************/
/***************		Layer: MCAL				******************/
/***************		SWC: Timer				******************/
/***************		Date: 2-11-2023			******************/
/***************		Version: 1.00			******************/
/*****************************************************************/
/*****************************************************************/


#include"../../LIB/STD_TYPES.h"
#include"../../LIB/BIT_MATH.h"

#include "TIMER_interface.h"
#include "TIMER_register.h"
#include "TIMER_config.h"
#include "TIMER_private.h"

static void (*TIMER0_pvOverflowCallBackFunc)(void)=NULL;

static void (*TIMER0_pvCompMatchCallBackFunc)(void)=NULL;

static void (*ICU_pvCallBackFunc)(void)=NULL;

void TIMER0_voidInt(void)
{
	SET_BIT(TCCR0,TCCR0_WGM00);
	SET_BIT(TCCR0,TCCR0_WGM01);

	OCR0=TIMER0_u8COMP_MATCH_VAL;

	TCCR0 &=TIMER0_PRESCALER_MASK;
	TCCR0 |=TIMER0_u8PRESCALER;

	SET_BIT(TCCR0,TCCR0_COM01);
	CLR_BIT(TCCR0,TCCR0_COM00);

}

u8 TIMER0_u8OverflowSetCallBack(void (*Copy_pvCallBackFunc)(void))
{
	u8 Local_u8ErrorState=OK;

	if(Copy_pvCallBackFunc != NULL)
	{
		TIMER0_pvOverflowCallBackFunc=Copy_pvCallBackFunc;
	}
	else
	{
		Local_u8ErrorState=NULL_POINTER;
	}


	return Local_u8ErrorState;
}

u8 TIMER0_u8CompMatchSetCallBack(void (*Copy_pvCallBackFunc)(void))
{
	u8 Local_u8ErrorState=OK;

	if(Copy_pvCallBackFunc != NULL)
	{
		TIMER0_pvCompMatchCallBackFunc=Copy_pvCallBackFunc;
	}
	else
	{
		Local_u8ErrorState=NULL_POINTER;
	}

	return Local_u8ErrorState;
}
void TIMER0_voidSetPreload(u8 Copy_u8Preload)
{
	TCNT0 = Copy_u8Preload;
}
void TIMER1_voidInt(void)
{
	CLR_BIT(TCCR1A,TCCR1A_WGM10);
	CLR_BIT(TCCR1A,TCCR1A_WGM11);
	CLR_BIT(TCCR1B,TCCR1B_WGM12);
	CLR_BIT(TCCR1B,TCCR1B_WGM13);

	CLR_BIT(TCCR1B,TCCR1B_CS10);
	SET_BIT(TCCR1B,TCCR1B_CS11);
	CLR_BIT(TCCR1B,TCCR1B_CS12);
}
void TIMER1_voidSetChannelACompValue(u16 Copy_u16Value)
{
	OCR1A=Copy_u16Value;
}
void TIMER1_voidSetPreloadValue(u16 Copy_u16Value)
{
	TCNT1=Copy_u16Value;
}
u16 TIMER1_Readu16TimerValue(void)
{
	return TCNT1;
}
void ICU_voidInit(void)
{
	CLR_BIT(TCCR1A,TCCR1A_WGM10);
	CLR_BIT(TCCR1A,TCCR1A_WGM11);
	CLR_BIT(TCCR1B,TCCR1B_WGM12);
	CLR_BIT(TCCR1B,TCCR1B_WGM13);

	CLR_BIT(TCCR1B,TCCR1B_CS10);
	SET_BIT(TCCR1B,TCCR1B_CS11);
	CLR_BIT(TCCR1B,TCCR1B_CS12);

	SET_BIT(TCCR1B,TCCR1B_ICES1);
	TCNT1 = 0;
	ICR1 = 0;

	SET_BIT(TIMSK,TIMSK_TICIE1);

}
u8 ICU_u8SetCallBack(void(*Copy_pvCallBackFunc)(void))
{
	u8 Local_u8ErrorState = OK;

	if(Copy_pvCallBackFunc != NULL)
	{
		ICU_pvCallBackFunc = Copy_pvCallBackFunc;
	}
	else
	{
		Local_u8ErrorState=NULL_POINTER;
	}

	return Local_u8ErrorState;
}
u8 ICU_u8SetTrigger(u8 Copy_u8TriggerSource)
{
	u8 Local_u8ErrorState=OK;

	switch(Copy_u8TriggerSource)
	{

	case FALLING_EDGE : CLR_BIT(TCCR1B,TCCR1B_ICES1); break;
	case RISING_EDGE  : SET_BIT(TCCR1B,TCCR1B_ICES1); break;
	default : Local_u8ErrorState = NOK; break;
	}

	return Local_u8ErrorState;
}
u16 ICU_u16ReadInputCapture(void)
{
	return ICR1;
}
void ICU_voidInterruptDisable(void)
{
	CLR_BIT(TIMSK,TIMSK_TICIE1);
}

void WDT_voidEnable(void)
{
	SET_BIT(WDTCR,WDTCR_WDE);
}

void WDT_voidDisable(void)
{
	WDTCR |= WDT_MASK_DISABLE;
	WDTCR = 0;
}

u8 WDT_u8Sleep(u8 Copy_u8SleepTime)
{
	u8 Local_u8ErrorState=OK;
	if(Copy_u8SleepTime<8)
	{
		WDTCR &=WDT_PRESCALER_MASK;
		WDTCR |= Copy_u8SleepTime ;
	}
	else
	{
		Local_u8ErrorState=NOK;
	}

	return Local_u8ErrorState;
}


void __vector_11 (void)  __attribute__((signal));
void __vector_11 (void)
{

	if (TIMER0_pvOverflowCallBackFunc !=NULL)
	{
		TIMER0_pvOverflowCallBackFunc();
	}
	else
	{

	}
}
void __vector_10 (void)  __attribute__((signal));
void __vector_10 (void)
{

	if (TIMER0_pvCompMatchCallBackFunc != NULL)
	{
		TIMER0_pvCompMatchCallBackFunc();
	}
	else
	{

	}
}

void __vector_6(void) __attribute__((signal));
void __vector_6(void)
{
	if (ICU_pvCallBackFunc != NULL)
	{
		ICU_pvCallBackFunc();
	}
	else
	{

	}
}
