#ifndef __MYSPI_H
#define __MYSPI_H

uint8_t MySPI_R_MISO(void);

void MySPI_Init(void);


void MySPI_Start(void);

void MySPI_Stop(void);

uint8_t MySPI_SwapByte(uint8_t ByteSend);


#endif
