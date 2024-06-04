#ifndef __ESP8266_H
#define __ESP8266_H
#include "stm32f10x.h"                  // Device header

#define WIFI_NAME 	"Redmi"
#define WIFI_PWD	"qq2213216806"

#define SERVER_IP  	 "192.168.239.248"
#define SERVER_PORT 	8888

void ESP8266_Init(void);
void ESP8266_Printf(char *format, ...);

void ESP8266_Send(char *data,uint8_t Len);
#endif /*__ESP8266_H*/
