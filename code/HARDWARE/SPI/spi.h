#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

	 
#define SPI2_Init SPIx_Init    
#define SPI2_SetSpeed SPIx_SetSpeed     
#define SPI2_ReadWriteByte SPIx_ReadWriteByte 
 				  	    													  
void SPI2_Init(void);			 
void SPI2_SetSpeed(u8 SpeedSet); 
u8 SPI2_ReadWriteByte(u8 TxData);

#endif

