#include "ESP8266.h"
#include "stm32f10x.h"                  // Device header
#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "Serial.h"

void USART_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void USRT_Init(void)
{
	//开启外设时钟     使用UASRT3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//GPIO配置
	GPIO_InitTypeDef GPIOInitstruct;
	GPIOInitstruct.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIOInitstruct.GPIO_Pin =  GPIO_Pin_10; //UART3_TX PB10
	GPIOInitstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitstruct);
	
	GPIOInitstruct.GPIO_Mode = GPIO_Mode_IPU; 
	GPIOInitstruct.GPIO_Pin =  GPIO_Pin_11; //UART3_RX PB11
	GPIO_Init(GPIOB,&GPIOInitstruct);
	
	//UASR3 配置
	USART_InitTypeDef USARTInitstruct;
	USARTInitstruct.USART_BaudRate = 115200;
	USARTInitstruct.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;
	USARTInitstruct.USART_Parity = USART_Parity_No;
	USARTInitstruct.USART_StopBits =  USART_StopBits_1;
	USARTInitstruct.USART_WordLength = USART_WordLength_8b;
	USARTInitstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3,&USARTInitstruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_NVIC_Init();
	
	USART_Cmd(USART3,ENABLE);
		
	
}

static void ESP8266_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

static void ESP8266_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		ESP8266_SendByte(String[i]);
	}
}

void ESP8266_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	ESP8266_SendString(String);
}

static void ESP8266_Delay(uint16_t time)
{
	u16 i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--);    
   }
}

void ESP8266_Init(void)
{
	USRT_Init();
	//ESP8266 初始配置
	ESP8266_Printf("AT+RST\r\n");  //esp8266重启
	ESP8266_Delay(2500);
	ESP8266_Printf("AT+CWMODE=1\r\n"); //将8266设置为STA模式
	ESP8266_Delay(500);
	ESP8266_Printf("AT+RST\r\n");  //esp8266重启
	ESP8266_Delay(2500);
	ESP8266_Printf("AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_PWD); //连接wifi
	ESP8266_Delay(5000);
	ESP8266_Printf("AT+CIPMUX=1\r\n"); //启动多连接
	ESP8266_Delay(500);
	ESP8266_Printf("AT+CIPSERVER=1\r\n");//建立server
	ESP8266_Delay(500);
	ESP8266_Printf("AT+CIPSTART=0,\"TCP\",\"%s\",%d\r\n",SERVER_IP,SERVER_PORT); //连接TCP服务器
	ESP8266_Delay(500);
}
void ESP8266_Send(char *data,uint8_t Len)
{
	ESP8266_Printf("AT+CIPSEND=0,%d\r\n",Len);
	printf("AT+CIPSEND=0,%d\r\n",Len);
	ESP8266_Printf("%s\r\n",data);
	printf("%s\r\n",data);
}