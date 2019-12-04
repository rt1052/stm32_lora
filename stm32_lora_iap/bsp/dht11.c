#include "bsp.h"

#define DHT11_GPIO_TYPE  GPIOB
#define DHT11_GPIO_PIN   GPIO_Pin_6

#define DHT11_OUT_H GPIO_SetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_OUT_L GPIO_ResetBits(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)
#define DHT11_IN    GPIO_ReadInputDataBit(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)

void dht11_gpio_input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(DHT11_GPIO_TYPE, &GPIO_InitStructure);
}

void dht11_gpio_output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(DHT11_GPIO_TYPE, &GPIO_InitStructure);
}

void dht11_reset(void)
{
    dht11_gpio_output();
    DHT11_OUT_L;
    delay_us(19000);
    DHT11_OUT_H;
    delay_us(30);
    dht11_gpio_input();
}

u16 dht11_scan(void)
{
    return DHT11_IN;
}

u16 dht11_read_bit(void)
{
    while (DHT11_IN == RESET);
    delay_us(40);
    if (DHT11_IN == SET)
    {
        while (DHT11_IN == SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

u16 dht11_read_byte(void)
{
    u16 i;
    u16 data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

bool dht11_read_data(uint8_t *buf)
{    
    dht11_reset();
    if (dht11_scan() == RESET)
    {
        while (dht11_scan() == RESET);
        while (dht11_scan() == SET);

        for (uint8_t i = 0; i < 5; i++) {
            buf[i] = dht11_read_byte();
        }
        
        while (dht11_scan() == RESET);
        dht11_gpio_output();
        DHT11_OUT_H;
        
        if (buf[4] == check_sum(buf, 4)) {
            return true;
        } else {
            memset(buf, 0, 5);
        }
    }
    
    return false;
}

