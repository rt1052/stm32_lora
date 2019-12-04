/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */


#include "sx1276-Hal.h"
#include "bsp.h"
   
/*!
 * SX1276 RESET I/O definitions
 */
#define RESET_IOPORT                                GPIOA
#define RESET_PIN                                   GPIO_Pin_3

/*!
 * SX1276 SPI NSS I/O definitions
 */
#define NSS_IOPORT                                  GPIOA
#define NSS_PIN                                     GPIO_Pin_4


void SX1276InitIo( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  RESET_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(RESET_IOPORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin =  NSS_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(NSS_IOPORT, &GPIO_InitStructure);	
}

void SX1276SetReset( uint8_t state )
{
    if( state == RADIO_RESET_ON )
    {
        GPIO_ResetBits(RESET_IOPORT, RESET_PIN);
    }
    else
    {
        GPIO_SetBits(RESET_IOPORT, RESET_PIN);
    }

}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size)
{
    uint8_t i;
	
    GPIO_ResetBits(NSS_IOPORT, NSS_PIN);
    
    spi_read_write_byte( addr | 0x80 );
    for (i = 0; i < size; i++ ) {
        spi_read_write_byte(buffer[i]);
    }
    
    GPIO_SetBits(NSS_IOPORT, NSS_PIN);
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    GPIO_ResetBits(NSS_IOPORT, NSS_PIN);

    spi_read_write_byte( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = spi_read_write_byte( 0 );
    }

    GPIO_SetBits(NSS_IOPORT, NSS_PIN);
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

uint8_t SX1276ReadDio0( void )
{
    //return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
    return 0;
}

uint8_t SX1276ReadDio1( void )
{
    //return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
    return 0;  
}

uint8_t SX1276ReadDio2( void )
{
    //return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
    return 0;  
}

uint8_t SX1276ReadDio3( void )
{
    //return IoePinGet( RF_DIO3_PIN );
    //return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );
    return 0;  
}

uint8_t SX1276ReadDio4( void )
{
    //return IoePinGet( RF_DIO4_PIN );
    //return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );
    return 0;  
}

uint8_t SX1276ReadDio5( void )
{
    //return IoePinGet( RF_DIO5_PIN );
    //return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );
    return 0;  
}

void SX1276WriteRxTx( uint8_t txEnable )
{
    //u_printf("*********SX1276WriteRxTx en:%d\r\n", txEnable);
}
