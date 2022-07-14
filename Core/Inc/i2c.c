/*****************************************************************************
*                                                                                                                                                         *
*                        File:    软件模拟IIC, HAL库版本，标准库也                                                     *
*                                    可用，只需要做一些相对应修改即可                                                     *
*                                                                                                                                                         *
*****************************************************************************/

#include "i2c.h"
#include "delay.h"

/*******************************************************************************
* 函 数 名         : i2c_stare
* 函数功能         : iic开始信号,SCL高电平时，SDA出现一个下跳沿表示启动信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void i2c_stare(void) {
    SDA_OUT();
    I2C_SDA_UP;
    I2C_SCL_UP;
    delay_us(4);
    I2C_SDA_LOW;        //启动开始信号
    delay_us(4);
    I2C_SCL_LOW;      //钳住I2C总线，准备发送或接收数据
}


/*******************************************************************************
* 函 数 名         : i2c_stop
* 函数功能         : iic停止信号,SCL高电平时，SDA出现一个上跳沿表示停止信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void i2c_stop(void) {
    SDA_OUT();            //sda线输出
    I2C_SDA_LOW;         //发送结束条件的数据信号
    I2C_SCL_UP;
    delay_us(4);         //结束条件建立时间大于4μ
    I2C_SDA_UP;         //发送I2C总线结束信号
    delay_us(4);
    I2C_SCL_LOW;
}


/*******************************************************************************
* 函 数 名         : i2c_send
* 函数功能         : iic发送数据
* 输    入         : uint8_t dat,要发送的数据
* 输    出         : 无
*******************************************************************************/
void i2c_send(uint8_t dat) {

    unsigned char temp;
    SDA_OUT();
    for (temp = 0x80; temp != 0; temp >>= 1) {
        if ((temp & dat) == 0) {
            I2C_SDA_LOW;
        } else {
            I2C_SDA_UP;
        }
        delay_us(1);
        I2C_SCL_UP;
        delay_us(4);
        I2C_SCL_LOW;
    }
}


/*******************************************************************************
* 函 数 名         : i2c_read
* 函数功能         : iic接收数据
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
uint8_t i2c_read(void) {
    unsigned char temp;
    unsigned char dat;
    I2C_SDA_UP;                     //释放总线准备接收
    SDA_IN();
    for (temp = 0x80; temp != 0; temp >>= 1) {
        delay_us(1);
        I2C_SCL_UP;
        if (I2C_SDA == 1) {
            dat |= temp;
        } else {
            dat &= ~temp;
        }
        I2C_SCL_LOW;
    }
    return dat;
}


/*******************************************************************************
* 函 数 名         : i2c_wit_ack
* 函数功能         : iic等待应答
* 输    入         : 无
* 输    出         : 0/1，返回1表示无应答信号，返回0表示应答
*******************************************************************************/
char i2c_wit_ack(void) {

    uint8_t con = 0;

    I2C_SDA_UP;       //释放数据线，准备接收应答
    delay_us(1);
    I2C_SCL_UP;                //CPU驱动SCL = 1, 此时器件会返回ACK应答
    SDA_IN();
    delay_us(1);

    while (I2C_SDA)    //CPU读取SDA口线状态
    {
        con++;
        if (con > 255) {
            i2c_stop();
            return 1;     //无应答信号
        }
    }

    I2C_SCL_LOW;
    delay_us(1);
    return 0;             //有应答
}

/*******************************************************************************
* 函 数 名         : i2c_ack
* 函数功能         : iic应答信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void i2c_ack(void) {
    SDA_OUT();
    I2C_SDA_LOW;
    delay_us(1);
    I2C_SCL_UP;            //CPU产生1个时钟
    delay_us(4);         //时钟低电平周期大于4μ
    I2C_SCL_LOW;         //清时钟线，钳住I2C总线以便继续接收
    delay_us(4);
    I2C_SDA_UP;            //CPU释放SDA总线
}


/*******************************************************************************
* 函 数 名         : I2C_No_ack
* 函数功能         : iic非应答信号
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void I2C_No_ack(void) {
    SDA_OUT();
    I2C_SDA_UP;
    delay_us(1);
    I2C_SCL_UP;
    delay_us(4);
    I2C_SCL_LOW;
    delay_us(4);
}


/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能         : SDA 设置为输出
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(void) {

/*    标准库使用，修改端口引脚可用
  GPIO_InitTypeDef GPIO_InitTypeStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitTypeStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitTypeStruct.GPIO_Pin=GPIO_Pin_7;
  GPIO_InitTypeStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitTypeStruct);
*/

    /*    HAL库使用，HAL库注意要把初始化函数的静态标记去掉    */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = QSDA_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(QSDA_GPIO_Port, &GPIO_InitStruct);
}


/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能         : SDA 设置为输入
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(void) {

/*    标准库使用，修改端口引脚可用
  GPIO_InitTypeDef GPIO_InitTypeStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitTypeStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_InitTypeStruct.GPIO_Pin=GPIO_Pin_7;
  GPIO_InitTypeStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitTypeStruct);
*/

    /*    HAL库使用, HAL库注意要把初始化函数的静态标记去掉    */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = QSDA_Pin;                    //使用STM3cubemx是定义好的SDA_Pin为GPIO_PIN_8的标签
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(QSDA_GPIO_Port, &GPIO_InitStruct);
}
