#include "mm32f5270.h"
#include <stdint.h>
#include <stdio.h>
#include "delay.h"
#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "UART.h"

u8 h,t;
u8 humiH[1];
u8 tempH[1];
u8 humiL[1];
u8 tempL[1];

uint32_t AD0, AD1, AD2, AD3, AD4, AD6;	//定义AD值变量
float CO,CH4,CO1,CH41,C3H8,C3H81,gm,gm1,H2S;
float Data_CO()
  {
	 ADC_ChannelCmd(ADC1, ADC_Channel_0, DISABLE);
	 AD0 = AD_GetValue(ADC_Channel_0);
	 ADC_ChannelCmd(ADC1, ADC_Channel_0, ENABLE);
	 CO1=AD2*0.001;
	if(CO1<=2.5)
	{
		CO=(CO1/2.5)*40+10;
	}
	if(CO1>2.5&&CO1<4)
	{
		CO=((CO1-2.5)/(4-2.5))*150+50;
	}
	if(CO1>=4&&CO1<=4.5)
	{
	 CO=((CO1-4)/(4.5-4))*300+200;
	}
	if(CO1>4.5)
	{
		CO=500;
	}
	return CO;
 }
float Data_CH4()
 {
	ADC_ChannelCmd(ADC1, ADC_Channel_2, ENABLE);
  AD2 = AD_GetValue(ADC_Channel_2);
	ADC_ChannelCmd(ADC1, ADC_Channel_2, DISABLE);
	CH41=AD4*0.001;
	if(CH41<=1.3)
	{
		CH4=CH41*192;
	}
	if(CH41>1.3&&CH41<3)
	{
		CH4=(CH41-1.3)*2850/1.7+250;
	}
	if(CH41>=3&&CH41<=3.7)
	{
	 CH4=CH41*10000-27000;
	}
	if(CH41>3.7)
	{
		CH4=10000;
	}
	return CH4;
 }
float Data_C3H8()
{
  ADC_ChannelCmd(ADC1, ADC_Channel_4, ENABLE);
  AD4 = AD_GetValue(ADC_Channel_4);
	ADC_ChannelCmd(ADC1, ADC_Channel_4, DISABLE);
	C3H81=AD1*0.001;
	if(C3H81<1.5)
	{
	  C3H8=C3H81*200;
	}
	if(C3H81>1.5&&C3H81<4)
	{
	  C3H8=C3H81*1200-1800+300;
	}
  if(C3H81>=4&&C3H81<4.5)
	{
	 C3H8=C3H81*11000-44000+4500;
	}
	if(C3H81>=4.5)
	{
	 C3H8=10000;
	}
	return C3H8;
}
float Data_gm()
{
  ADC_ChannelCmd(ADC1, ADC_Channel_1, ENABLE);
  AD1 = AD_GetValue(ADC_Channel_1);
	ADC_ChannelCmd(ADC1, ADC_Channel_1, DISABLE);
	gm=100-(AD6/50);
	return gm;
}
float Data_H2S()
{
  ADC_ChannelCmd(ADC1, ADC_Channel_6, ENABLE);
  AD6 = AD_GetValue(ADC_Channel_6);
	ADC_ChannelCmd(ADC1, ADC_Channel_6, DISABLE);
	H2S=AD1/200;
	return H2S;
}
int main()
{
	
	Delay_Init();
	LED_Init();
	DHT11_Init();
	ADC1_Init();
  LCD_GPIO_Init();
	LCD_Init();	
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);	
	uart_nvic_init(9600);
  
   
   
    
		
	
  
	LCD_ShowString(1,1,"TEMP:",BLACK,WHITE,16,0);
	LCD_ShowString(1,16,"HUMI:",BLACK,WHITE,16,0);
	LCD_ShowString(1,32,"CO:",BLACK,WHITE,16,0);
	LCD_ShowString(1,48,"CH4:",BLACK,WHITE,16,0);
  LCD_ShowString(1,64,"C3H8",BLACK,WHITE,16,0);
  LCD_ShowString(1,80,"GM:",BLACK,WHITE,16,0);
	LCD_ShowString(1,96,"H2S:",BLACK,WHITE,16,0);
	Serial_SendByte(1);
	while(1)
	{
		if(DHT11_Read_Data1(humiH,humiL,tempH,tempL)==0)
		{
		  t=tempH[0]*10+tempL[0];
			h=humiH[0]*10+humiL[0];
		}
		
		LCD_ShowIntNum(50,1,t,7,BLACK,WHITE,16);
		LCD_ShowIntNum(50,16,h,7,BLACK,WHITE,16);
		
		LCD_ShowIntNum(50,32,Data_CO(),7,BLACK,WHITE,16);
    LCD_ShowIntNum(50,48,Data_CH4(),7,BLACK,WHITE,16);
	  LCD_ShowIntNum(50,64,Data_C3H8(),7,BLACK,WHITE,16);
		LCD_ShowIntNum(50,80,Data_gm(),7,BLACK,WHITE,16);
		LCD_ShowIntNum(50,96,Data_H2S(),7,BLACK,WHITE,16);

////		GPIO_ResetBits(GPIOD,GPIO_Pin_10);
////		GPIO_ResetBits(GPIOD,GPIO_Pin_11);
////		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
////		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
////		Delay_Ms(10000);
////		GPIO_SetBits(GPIOD,GPIO_Pin_10);
////		GPIO_SetBits(GPIOD,GPIO_Pin_11);
////		GPIO_SetBits(GPIOD,GPIO_Pin_12);
////		GPIO_SetBits(GPIOD,GPIO_Pin_13);
////		Delay_Ms(10000);

	}
}

