#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "TIMERS_Interface.h"
#include "TIMERS_Register.h"
#include "GIE_interface.h"


#include "LCD_interface.h"

volatile u8 R1=0, R2=0, R3=0, OnTick=0, PeriodTime=0;
volatile u8 test=0, TC=0;

void App (void)
{

	volatile static u8 counter =0;
	counter ++;
	if(counter == 1)
	{
		R1=Timer1_ReadInputCaptureValue();
//		R1= 0;

	}
	else if(counter ==2)
	{
		R2 = Timer1_ReadInputCaptureValue();
		Timer1_InputCaptureEdge(FALLING);
		PeriodTime =R2-R1-1;
	}

	else if(counter==3)
	{
		R3 = Timer1_ReadInputCaptureValue();
		OnTick=R3-R2-1;
		Timer1_ICU_InterruptDisable();

	}
}

void main(void)
{


	DIO_u8SetPinDirection(DIO_u8_PORTD,DIO_u8_PIN0,DIO_u8_PIN_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8_PORTA,DIO_u8_PIN1,DIO_u8_PIN_OUTPUT);

	DIO_u8SetPinDirection(DIO_u8_PORTB,DIO_u8_PIN3,DIO_u8_PIN_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8_PORTD,DIO_u8_PIN6,DIO_u8_PIN_INPUT);

	HLCD_VoidInit();


	TIMER0_CTC_SetOCR0(200);
	TIMER0_Init(TIMER0_FASTPWM_MODE, TIMER0_SCALER_8);
	Timer1_Init(TIMER1_NORMAL_MODE,TIMER1_SCALER_8);
	Timer1_InputCaptureEdge(RISING);

	Timer1_ICU_InterruptEnable();
	Timer1_ICU_SetCallBack(&App);
	GIE_voidEnable();




	HLCD_VoidGotoXY(0,0);
	HLCD_VoidSendString((u8*)"On Ticks=");
	HLCD_VoidSendNumber(OnTick);

	HLCD_VoidGotoXY(1,0);
	HLCD_VoidSendString((u8*)"Period Ticks=");
	HLCD_VoidSendNumber(PeriodTime);

	while(1)
	{




	}

}

