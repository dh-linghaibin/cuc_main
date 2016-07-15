/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�
*
* �ļ���ʶ��
* ժ    Ҫ��
*
*
* ��ǰ�汾��
* ��    �ߣ�lhb Steven
* ������ڣ�2016/7/13
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#ifndef __MOTO_H
#define __MOTO_H

#include "Type.h"

typedef struct {
    u8   bit0:1;
    u8   bit1:1;
    u8   bit2:1;
    u8   bit3:1;
    u8   bit4:1;
    u8   bit5:1;
    u8   bit6:1;
    u8   bit7:1;
}bitu8;

/*�������*/
typedef struct moto{
    u8 sleep_start;//�Ƿ������
    u16 sleep_count;//�ٶȼ���
    u16 sleep;//����ٶ�
    u8 flag;//�����ĸ�����
}moto;
/*�����������*/
typedef struct stepping{
    u8 en;//ʹ�ܲ������
    u16 sleep_count;//����ٶȼ���
    u16 sleep;//����ٶ�
    u16 step;//���λ��
    u8 dir;//�������
    u8 dr;//���巽��
    u8 step_count;//���ٶ�
}stepping;
/**********************************************��������***************************************************** 
* ��������: void MotoInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/13
************************************************************************************************************/ 
void MotoInit(void);

#endif


