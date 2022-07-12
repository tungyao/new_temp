
#include "stm32f0xx.h"
#include "AHT20.h"


void Delay_N10us(uint32_t t)//延时函数
{
    uint32_t k;

    while (t--) {
        for (k = 0; k < 2; k++);//110
    }
}

void SensorDelay_us(uint32_t t)//延时函数
{

    for (t = t - 2; t > 0; t--) {
        Delay_N10us(1);
    }
}

void Delay_4us(void)        //延时函数
{
    Delay_N10us(1);
    Delay_N10us(1);
    Delay_N10us(1);
    Delay_N10us(1);
}

void Delay_5us(void)        //延时函数
{
    Delay_N10us(1);
    Delay_N10us(1);
    Delay_N10us(1);
    Delay_N10us(1);
    Delay_N10us(1);

}

void Delay_1ms(uint32_t t)        //延时函数
{
    HAL_Delay(1);
}


void AHT20_Clock_Init(void)        //延时函数
{
//    HAL_RCC_ClockConfig(RCC_APB2)
//    HAL_RCC_ClockConfig()
//    HAL_RCCEx_PeriphCLKConfig(RCC_AHBENR_GPIOBEN,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    HAL_RCC_ClockConfig(,ENABLE);
}

void SDA_Pin_Output_High(void)   //将PB15配置为输出 ， 并设置为高电平， PB15作为I2C的SDA
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;//开漏输出
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}

void SDA_Pin_Output_Low(void)  //将P15配置为输出  并设置为低电平
{

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;//开漏输出
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void SDA_Pin_IN_FLOATING(void)  //SDA配置为浮空输入
{

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;//开漏输出
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void SCL_Pin_Output_High(void) //SCL输出高电平，P14作为I2C的SCL
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void SCL_Pin_Output_Low(void) //SCL输出低电平
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

}

void Init_T_I2C_Sensor_Port(void) //初始化I2C接口,输出为高电平
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
//    GPIO_SetBits(GPIOB, GPIO_Pin_1);//输出高电平


    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void I2C_Start(void)         //I2C主机发送START信号
{
    SDA_Pin_Output_High();
    SensorDelay_us(8);
    SCL_Pin_Output_High();
    SensorDelay_us(8);
    SDA_Pin_Output_Low();
    SensorDelay_us(8);
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
}


void AHT20_WR_Byte(uint8_t Byte) //往AHT20写一个字节
{
    uint8_t Data, N, i;
    Data = Byte;
    i = 0x80;
    for (N = 0; N < 8; N++) {
        SCL_Pin_Output_Low();
        Delay_4us();
        if (i & Data) {
            SDA_Pin_Output_High();
        } else {
            SDA_Pin_Output_Low();
        }

        SCL_Pin_Output_High();
        Delay_4us();
        Data <<= 1;

    }
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    SDA_Pin_IN_FLOATING();
    SensorDelay_us(8);

}


uint8_t AHT20_RD_Byte(void)//从AHT20读取一个字节
{
    uint8_t Byte, i, a;
    Byte = 0;
    SCL_Pin_Output_Low();
    SDA_Pin_IN_FLOATING();
    SensorDelay_us(8);
    for (i = 0; i < 8; i++) {
        SCL_Pin_Output_High();
        Delay_5us();
        a = 0;

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) a = 1;
        Byte = (Byte << 1) | a;
        SCL_Pin_Output_Low();
        Delay_5us();
    }
    SDA_Pin_IN_FLOATING();
    SensorDelay_us(8);
    return Byte;
}


uint8_t Receive_ACK(void)   //看AHT20是否有回复ACK
{
    uint16_t CNT;
    CNT = 0;
    SCL_Pin_Output_Low();
    SDA_Pin_IN_FLOATING();
    SensorDelay_us(8);
    SCL_Pin_Output_High();
    SensorDelay_us(8);
    while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) && CNT < 100)
        CNT++;
    if (CNT == 100) {
        return 0;
    }
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    return 1;
}

void Send_ACK(void)          //主机回复ACK信号
{
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    SDA_Pin_Output_Low();
    SensorDelay_us(8);
    SCL_Pin_Output_High();
    SensorDelay_us(8);
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    SDA_Pin_IN_FLOATING();
    SensorDelay_us(8);
}

void Send_NOT_ACK(void)    //主机不回复ACK
{
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    SDA_Pin_Output_High();
    SensorDelay_us(8);
    SCL_Pin_Output_High();
    SensorDelay_us(8);
    SCL_Pin_Output_Low();
    SensorDelay_us(8);
    SDA_Pin_Output_Low();
    SensorDelay_us(8);
}

void Stop_I2C(void)      //一条协议结束
{
    SDA_Pin_Output_Low();
    SensorDelay_us(8);
    SCL_Pin_Output_High();
    SensorDelay_us(8);
    SDA_Pin_Output_High();
    SensorDelay_us(8);
}

uint8_t AHT20_Read_Status(void)//读取AHT20的状态寄存器
{

    uint8_t Byte_first;
    I2C_Start();
    AHT20_WR_Byte(0x71);
    Receive_ACK();
    Byte_first = AHT20_RD_Byte();
    Send_NOT_ACK();
    Stop_I2C();
    return Byte_first;
}

void AHT20_SendAC(void) //向AHT20发送AC命令
{

    I2C_Start();
    AHT20_WR_Byte(0x70);
    Receive_ACK();
    AHT20_WR_Byte(0xac);//0xAC采集命令
    Receive_ACK();
    AHT20_WR_Byte(0x33);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    Stop_I2C();

}

//CRC校验类型：CRC8/MAXIM
//多项式：X8+X5+X4+1
//Poly：0011 0001  0x31
//高位放到后面就变成 1000 1100 0x8c
//C现实代码：
uint8_t Calc_CRC8(uint8_t *message, uint8_t Num) {
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    for (byte = 0; byte < Num; byte++) {
        crc ^= (message[byte]);
        for (i = 8; i > 0; --i) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else crc = (crc << 1);
        }
    }
    return crc;
}

void AHT20_Read_CTdata(uint32_t *ct) //没有CRC校验，直接读取AHT20的温度和湿度数据
{
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;
    AHT20_SendAC();//向AHT20发送AC命令
    Delay_1ms(80);//延时80ms左右
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    {
        SensorDelay_us(1508);
        if (cnt++ >= 100) {
            break;
        }
    }
    I2C_Start();
    AHT20_WR_Byte(0x71);
    Receive_ACK();
    Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    Send_ACK();
    Byte_2th = AHT20_RD_Byte();//湿度
    Send_ACK();
    Byte_3th = AHT20_RD_Byte();//湿度
    Send_ACK();
    Byte_4th = AHT20_RD_Byte();//湿度/温度
    Send_ACK();
    Byte_5th = AHT20_RD_Byte();//温度
    Send_ACK();
    Byte_6th = AHT20_RD_Byte();//温度
    Send_NOT_ACK();
    Stop_I2C();

    RetuData = (RetuData | Byte_2th) << 8;
    RetuData = (RetuData | Byte_3th) << 8;
    RetuData = (RetuData | Byte_4th);
    RetuData = RetuData >> 4;
    ct[0] = RetuData;//湿度
    RetuData = 0;
    RetuData = (RetuData | Byte_4th) << 8;
    RetuData = (RetuData | Byte_5th) << 8;
    RetuData = (RetuData | Byte_6th);
    RetuData = RetuData & 0xfffff;
    ct[1] = RetuData; //温度

}


void AHT20_Read_CTdata_crc(uint32_t *ct) //CRC校验后，读取AHT20的温度和湿度数据
{
    volatile uint8_t Byte_1th = 0;
    volatile uint8_t Byte_2th = 0;
    volatile uint8_t Byte_3th = 0;
    volatile uint8_t Byte_4th = 0;
    volatile uint8_t Byte_5th = 0;
    volatile uint8_t Byte_6th = 0;
    volatile uint8_t Byte_7th = 0;
    uint32_t RetuData = 0;
    uint16_t cnt = 0;
    // uint8_t  CRCDATA=0;
    uint8_t CTDATA[6] = {0};//用于CRC传递数组

    AHT20_SendAC();//向AHT20发送AC命令
    Delay_1ms(80);//延时80ms左右
    cnt = 0;
    while (((AHT20_Read_Status() & 0x80) == 0x80))//直到状态bit[7]为0，表示为空闲状态，若为1，表示忙状态
    {
        SensorDelay_us(1508);
        if (cnt++ >= 100) {
            break;
        }
    }

    I2C_Start();

    AHT20_WR_Byte(0x71);
    Receive_ACK();
    CTDATA[0] = Byte_1th = AHT20_RD_Byte();//状态字，查询到状态为0x98,表示为忙状态，bit[7]为1；状态为0x1C，或者0x0C，或者0x08表示为空闲状态，bit[7]为0
    Send_ACK();
    CTDATA[1] = Byte_2th = AHT20_RD_Byte();//湿度
    Send_ACK();
    CTDATA[2] = Byte_3th = AHT20_RD_Byte();//湿度
    Send_ACK();
    CTDATA[3] = Byte_4th = AHT20_RD_Byte();//湿度/温度
    Send_ACK();
    CTDATA[4] = Byte_5th = AHT20_RD_Byte();//温度
    Send_ACK();
    CTDATA[5] = Byte_6th = AHT20_RD_Byte();//温度
    Send_ACK();
    Byte_7th = AHT20_RD_Byte();//CRC数据
    Send_NOT_ACK();                           //注意: 最后是发送NAK
    Stop_I2C();

    if (Calc_CRC8(CTDATA, 6) == Byte_7th) {
        RetuData = (RetuData | Byte_2th) << 8;
        RetuData = (RetuData | Byte_3th) << 8;
        RetuData = (RetuData | Byte_4th);
        RetuData = RetuData >> 4;
        ct[0] = RetuData;//湿度
        RetuData = 0;
        RetuData = (RetuData | Byte_4th) << 8;
        RetuData = (RetuData | Byte_5th) << 8;
        RetuData = (RetuData | Byte_6th);
        RetuData = RetuData & 0xfffff;
        ct[1] = RetuData; //温度

    } else {
        ct[0] = 0x00;
        ct[1] = 0x00;//校验错误返回值，客户可以根据自己需要更改
    }//CRC数据
}


void JH_Reset_REG(uint8_t addr) {

    uint8_t Byte_first, Byte_second, Byte_third, Byte_fourth;
    I2C_Start();
    AHT20_WR_Byte(0x70);//原来是0x70
    Receive_ACK();
    AHT20_WR_Byte(addr);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    AHT20_WR_Byte(0x00);
    Receive_ACK();
    Stop_I2C();

    Delay_1ms(5);//延时5ms左右
    I2C_Start();
    AHT20_WR_Byte(0x71);//
    Receive_ACK();
    Byte_first = AHT20_RD_Byte();
    Send_ACK();
    Byte_second = AHT20_RD_Byte();
    Send_ACK();
    Byte_third = AHT20_RD_Byte();
    Send_NOT_ACK();
    Stop_I2C();

    Delay_1ms(10);//延时10ms左右
    I2C_Start();
    AHT20_WR_Byte(0x70);///
    Receive_ACK();
    AHT20_WR_Byte(0xB0 | addr);////寄存器命令
    Receive_ACK();
    AHT20_WR_Byte(Byte_second);
    Receive_ACK();
    AHT20_WR_Byte(Byte_third);
    Receive_ACK();
    Stop_I2C();

    Byte_second = 0x00;
    Byte_third = 0x00;
}

void AHT20_Start_Init(void) {
    JH_Reset_REG(0x1b);
    JH_Reset_REG(0x1c);
    JH_Reset_REG(0x1e);
}

//int32_t main(void)
//{
//    uint32_t CT_data[2];
//	volatile int  c1,t1;
//	/***********************************************************************************/
//	/**///上电初始化SDA，SCL的IO口
//	/***********************************************************************************/
//	Init_I2C_Sensor_Port();
//	/***********************************************************************************/
//	/**///①刚上电，产品芯片内部就绪需要时间，延时100~500ms,建议500ms
//	/***********************************************************************************/
//	Delay_1ms(500);
//	/***********************************************************************************/
//	/**///②上电第一次发0x71读取状态字，判断状态字是否为0x18,如果不是0x18,进行寄存器初始化
//	/***********************************************************************************/
//	if((AHT20_Read_Status()&0x18)!=0x18)
//	{
//	AHT20_Start_Init(); //重新初始化寄存器
//	Delay_1ms(10);
//	}
//
//	/***********************************************************************************/
//	/**///③根据客户自己需求发测量命令读取温湿度数据，当前while（1）循环发测量命令读取温湿度数据，仅供参考
//	/***********************************************************************************/
//	while(1)
//	{
//	 AHT20_Read_CTdata(CT_data);       //不经过CRC校验，直接读取AHT20的温度和湿度数据    推荐每隔大于1S读一次
//    //AHT20_Read_CTdata_crc(CT_data);  //crc校验后，读取AHT20的温度和湿度数据
//

//	 c1 = CT_data[0]*100*10/1024/1024;  //计算得到湿度值c1（放大了10倍）
//	 t1 = CT_data[1]*200*10/1024/1024-500;//计算得到温度值t1（放大了10倍）
//	////下一步客户处理显示数据，
//	/***********************************************************************************/
//	/**///延时1.5s，建议间隔1.5s读一次
//	/***********************************************************************************/
//	delay_ms(1500);
//	 }

// }	
