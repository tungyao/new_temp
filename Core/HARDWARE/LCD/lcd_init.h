#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"
#define USE_HORIZONTAL 1  //???¨²?????????????? 0??1????? 2??3?????


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif
	


//-----------------LCD??????---------------- 

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)//SCL=SCLK
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)

#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)//SDA=MOSI
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)

#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)

#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)//DC
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)

#define LCD_CS_Clr()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)//CS
#define LCD_CS_Set()   HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)

#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)//BLK
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET)




void LCD_GPIO_Init(void);//?????GPIO
void LCD_Writ_Bus(u8 dat);//???SPI???
void LCD_WR_DATA8(u8 dat);//§Õ????????
void LCD_WR_DATA(u16 dat);//§Õ?????????
void LCD_WR_REG(u8 dat);//§Õ????????
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//??????????
void LCD_Init(void);//LCD?????
#endif




