#ifndef _I2C1_H
#define _I2C1_H
 
#include "ALL_Includes.h"
#include <stdio.h>
#include "HIFI.h"
#include "delay.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
   I2C_FAIL = 0,
   I2C_OK   = 1
}I2C_Status;

/* Exported constants --------------------------------------------------------*/
#define I2C_TIMING                                0x00310309
#define I2C_TIMEOUT                               0x10000
//extern cfg_reg r;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
I2C_Status I2C1_Read_Byte(uint8_t driver_Addr, uint8_t start_Addr, uint8_t *read_Buffer); 
I2C_Status I2C1_Write_Byte(uint8_t driver_Addr, uint8_t start_Addr, uint8_t write_data); 
extern void I2C_Configuration(void);
I2C_Status i2c_write(uint8_t *reg,uint8_t count);
extern void transmit_registers(cfg_reg const *r, int n);
extern void transmit_registers_R(cfg_reg const *r, int n);

#endif

