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
#include "Moto.h"

#define POWER_EN     PC_ODR_ODR5 //��Դ����

#define PLATEN_SLEEP PD_ODR_ODR2 //ѹֽ������
#define PLATEN_DIR   PC_ODR_ODR4 //ѹֽ������

#define CUTTER_SLEEP PD_ODR_ODR3 //��ֽ������
#define CUTTER_DIR   PC_ODR_ODR6 //��ֽ������

#define STEPPING_DIR    PC_ODR_ODR1 //�����������
#define STEPPING_SLEEP  PC_ODR_ODR2 //��������ٶ�

#define PHASE_EN    PE_CR2_C25 //������λʹ��ͬ��
/*�������*/
moto moto1;
/*�����������*/
stepping stepping1;
/**********************************************��������***************************************************** 
* ��������: void MotoInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/13
************************************************************************************************************/ 
void MotoInit(void) {
    PD_DDR_DDR2 = 1;//ѹֽ������
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 1;
    
    PD_DDR_DDR3 = 1;//��ֽ������
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 1;
    
    PD_DDR_DDR4 = 1;//������
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 1;
    
    PC_DDR_DDR4 = 1;//�̵���-ѹֽ����
    PC_CR1_C14 = 1;
    PC_CR2_C24 = 1;
    
    PC_DDR_DDR5 = 1;//�̵���-��Դ����
    PC_CR1_C15 = 1;
    PC_CR2_C25 = 1;
    
    PC_DDR_DDR6 = 1;//�̵���-��ֽ����
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 1;
    
    PC_DDR_DDR1 = 1;//�������-����
    PC_CR1_C11 = 1;
    PC_CR2_C21 = 1;
    
    PC_DDR_DDR2 = 1;//�������-����
    PC_CR1_C12 = 1;
    PC_CR2_C22 = 1;
    
    PB_DDR_DDR2 = 0;//ѹֽ����
    PB_CR1_C12 = 0;
    PB_CR2_C22 = 0;
    
    PB_DDR_DDR1 = 0;//ѹֽ����λ
    PB_CR1_C11 = 0;
    PB_CR2_C21 = 0;
    
    PB_DDR_DDR0 = 0;//��ֽ����λ--���������λ
    PB_CR1_C10 = 0;
    PB_CR2_C20 = 0;
    
    PB_DDR_DDR3 = 0;//��ֽ����λ
    PB_CR1_C13 = 0;
    PB_CR2_C23 = 0;
    
    PB_DDR_DDR4 = 0;//��ֽ����λ
    PB_CR1_C14 = 0;
    PB_CR2_C24 = 0;
    
    PE_DDR_DDR5 = 0;//������λͬ��
    PE_CR1_C15 = 0;
    PE_CR2_C25 = 0;//��Ҫ�ش�
    EXTI_CR2 |= BIT(1);//����PD���ж�
	EXTI_CR2 &= ~BIT(0);
    
    //��ʱ��1���ã�����1msһ�ε��ж�
	TIM1_PSCRH = 0X00;
	TIM1_PSCRL = 0Xc7;
	TIM1_ARRH = 0X00;
	TIM1_ARRL = 0X0a;
	TIM1_IER = 0X01;
	TIM1_CR1 = 0X01;
}
/**********************************************��������***************************************************** 
* ��������: void MotoSet(u8 moto, u8 sleep) 
* �������: u8 moto, u8 sleep 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/14
************************************************************************************************************/ 
void MotoSet(u8 moto, u8 sleep) { 
    PHASE_EN = 1;//ʹ�ܽ�����λͬ
    //ʹ�ܷ���
    //ʹ�ܵ�Դ
}

#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//����жϱ�־
    if(moto1.sleep_start == 1) {
        if(moto1.sleep_count < moto1.sleep) {
            moto1.sleep_count++;
        } else {
            moto1.sleep_count = 0;
            switch( moto1.flag ) {
                case 0:
                    moto1.flag = 1;
                    PLATEN_SLEEP = 1;
                    break;
                case 1:
                    moto1.flag = 2;
                    PLATEN_SLEEP = 0;
                    moto1.sleep = 50;//��������
                    break;
                case 2:
                    moto1.flag = 3;
                    PLATEN_SLEEP = 1;
                    moto1.sleep = 21;//��������
                    break;
                case 3:
                    moto1.flag = 0;
                    PLATEN_SLEEP = 0;
                    moto1.sleep_start = 0;//һ�����ڽ���
                    break;
                default:
                    moto1.flag = 0;
                    break;
            }
        }
    }
    //�������
    if(stepping1.en == 1) {
        if(stepping1.sleep_count < stepping1.sleep) {
            stepping1.sleep_count++;
        } else {
            stepping1.sleep_count = 0;
            //��������
            if(stepping1.dr == 0) {
                stepping1.dr = 1;
            } else {
                stepping1.dr = 0;
            }
            STEPPING_SLEEP = stepping1.dr;
            //������λ��
            if(stepping1.dir == 0) {
                stepping1.step++;
            } else {
                stepping1.step--;
            }
            //���ٶ�
            //ÿ20����һ��
            if(stepping1.step_count < 20) {
                stepping1.step_count++;
            } else {
                stepping1.step_count = 0;
                if(stepping1.sleep > 20) {
                    stepping1.sleep++;
                }
            }
        }
    }
    
    INTEN
}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void) {
    
}


#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void) {
    INTOFF
    //���յ�50HZ ��Ƶ�ź�
    moto1.sleep_start = 1;
    INTEN
    return;                    
}