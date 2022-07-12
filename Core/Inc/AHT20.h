//
// Created by tungyao on 2022/7/12.
//

#ifndef _AHT20_H_
#define _AHT20_H_

#include "stm32f0xx.h"

void Delay_N10us(uint32_t t);
void SensorDelay_us(uint32_t t);
void Delay_4us(void);
void Delay_5us(void);
void Delay_1ms(uint32_t t);
void AHT20_Clock_Init(void);
void SDA_Pin_Output_High(void)  ;
void SDA_Pin_Output_Low(void);
void SDA_Pin_IN_FLOATING(void);
void SCL_Pin_Output_High(void);
void SCL_Pin_Output_Low(void);
void Init_T_I2C_Sensor_Port(void);
void I2C_Start(void);
void AHT20_WR_Byte(uint8_t Byte);
uint8_t AHT20_RD_Byte(void);
uint8_t Receive_ACK(void);
void Send_ACK(void)	;
void Send_NOT_ACK(void);
void Stop_I2C(void);
uint8_t AHT20_Read_Status(void);
void AHT20_SendAC(void);
uint8_t Calc_CRC8(uint8_t *message,uint8_t Num);
void AHT20_Read_CTdata(uint32_t *ct);
void AHT20_Read_CTdata_crc(uint32_t *ct);
void JH_Reset_REG(uint8_t addr);
void AHT20_Start_Init(void);



#endif //NEW_TEMP_AHT20_H
