#ifndef _FLASH_H
#define _FLASH_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f0xx.h"


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* �궨�� --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)         //ҳ��С   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //���ʹ�С - 2�ֽ�
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01
#define FLASH_ADDR_FLAG           0x08003000     //FLASH��ַ - ��־λ
#define FLASH_ADDR_DATA           0x08003002     //FLASH��ַ - ����
#define LENGTH_DATA               1              //���ݳ���

                                                 //д�������
//uint16_t WriteBuf[LENGTH_DATA] = {0x1111, 0x2222, 0x3333, 0x4444, 0x5555};
//uint16_t ReadBuf[LENGTH_DATA];                   //����������
/* �������� ------------------------------------------------------------------*/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);


#endif /* _FLASH_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/