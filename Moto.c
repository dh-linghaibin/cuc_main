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
#include "Eeprom.h"
#include "Delay.h"
#include "Menu.h"
#include "Com.h"

#define POWER_EN     PC_ODR_ODR5 //��Դ����

#define PLATEN_SLEEP PD_ODR_ODR2 //ѹֽ������
#define PLATEN_DIR   PC_ODR_ODR4 //ѹֽ������
#define PLATEN_HD    PB_IDR_IDR2 //ѹֽ���ת���
#define PLATEN_UP    PB_IDR_IDR1 //ѹֽ���ת����λ

#define CUTTER_SLEEP PD_ODR_ODR3 //��ֽ������
#define CUTTER_DIR   PC_ODR_ODR6 //��ֽ������
#define CUTTER_UP    PB_IDR_IDR4 //��ֽ����λ
#define CUTTER_DOWN  PB_IDR_IDR3 //��ֽ����λ

#define STEPPING_DIR    PC_ODR_ODR1 //�����������
#define STEPPING_SLEEP  PC_ODR_ODR2 //��������ٶ�
#define STEPPING_ZERO   PB_IDR_IDR0 //����������

#define PHASE_EN    PE_CR2_C25 //������λʹ��ͬ��

#define CUTTER_LIGHT    PC_ODR_ODR3 //������ʹ��

#define SAFETY_LIGHT    PB_IDR_IDR5 //��ȫ��Ļ

#define MUSIC			PD_ODR_ODR4//������
/*�������*/
moto moto1;//ѹֽ���
moto moto2;//��ֽ���
/*�����������*/
stepping stepping1;
/*�Զ�ģʽ�ṹ��*/
autu autu_n;//�Զ��ṹ��

music music1;//������
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
    PB_CR1_C12 = 1;
    PB_CR2_C22 = 1;
    
    PB_DDR_DDR1 = 0;//ѹֽ����λ
    PB_CR1_C11 = 1;
    PB_CR2_C21 = 0;
    
    PB_DDR_DDR0 = 0;//��ֽ����λ--���������λ
    PB_CR1_C10 = 1;
    PB_CR2_C20 = 0;
    
    PB_DDR_DDR3 = 0;//��ֽ����λ
    PB_CR1_C13 = 0;
    PB_CR2_C23 = 1;
    
    PB_DDR_DDR4 = 0;//��ֽ����λ
    PB_CR1_C14 = 1;
    PB_CR2_C24 = 1;
    
    PE_DDR_DDR5 = 0;//������λͬ��
    PE_CR1_C15 = 0;
    PE_CR2_C25 = 0;//��Ҫ�ش�
    
    EXTI_CR2 |= BIT(1);//����PD���ж�
	EXTI_CR2 &= ~BIT(0);
    
    PC_DDR_DDR3 = 1;//������ʹ��
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    PB_DDR_DDR5 = 0;//��ȫ��Ļ
    PB_CR1_C15 = 0;
    PB_CR2_C25 = 0;
    
    CUTTER_SLEEP = 1;
    PLATEN_SLEEP = 1;
    
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 15;          //(15+1)��ƵΪ1M
    TIM1_ARRH = 0x1;
    TIM1_ARRL = 0xF4;        //ÿ500us�ж�һ��
    TIM1_IER = 0x01;         //��������ж�
    TIM1_CR1 = 0x00;         //������ʹ�ܣ���ʼ����
    
    //TIM2/3/4/5/6��ʹ����оƬ��Ӧ��ͷ�ļ�
    TIM2_PSCR_PSC = 4;          //2^4 ��ƵΪ1M 
    TIM2_ARRH = 0x17;
    TIM2_ARRL = 0xA0;        //ÿ4000us�ж�һ��
    TIM2_IER = 0x01;         //��������ж�
    //TIM2_CR1 = 0x00;         //������ʹ�ܣ���ʼ����
    
    
    CUTTER_LIGHT = 1;//�򿪼���ָʾ
    
    EXTI_CR1 &= ~BIT(2);//����PD���ж� 1 0
	EXTI_CR1 |= BIT(3);
    
    EXTI_CR1_PBIS = 2;    //PB�½��ش���
    //��һ�γ�ʼ��
    if(EepromRead(9) != 0x55) {
        EepromWrite(9,0x55);
        EepromWrite(10,0x00);
        EepromWrite(11,0x00);
//        //���ȳ�ʼ��
//        EepromWrite(12,0x04);
//        EepromWrite(13,0x04);
//        
//        EepromWrite(14,0x04);
    }
    EXTI_CR2_PEIS = 1;    //�����ش���
    
    stepping1.position = EepromRead(10);
    stepping1.position |= (u16)(EepromRead(11) << 8);
    stepping1.position_last = stepping1.position;
    
    autu_n.process = 0;
    autu_n.push_book = EepromRead(12);
    autu_n.specal_rst = EepromRead(13);
    autu_n.sui_cut_num = EepromRead(14);
    
    autu_n.sui_length = EepromRead(15);
    autu_n.sui_length |= (u16)(EepromRead(16) << 8);
    
    autu_n.sui_length = EepromRead(15);
    autu_n.sui_length |= (u16)(EepromRead(16) << 8);
    
    autu_n.sui_length_book = EepromRead(17);
    autu_n.sui_length_book |= (u16)(EepromRead(18) << 8);
    
    moto1.strength = EepromRead(19);
    moto1.strength |= (u16)(EepromRead(20) << 8);
    //����
    if(moto1.strength > 4000) {
        moto1.strength = 1000;
    } else if(moto1.strength < 1000) {
        moto1.strength = 1000;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoError(static void) 
* �������: static void 
* ���ز���: void  
* ��    ��: ���󱨾�  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/29
************************************************************************************************************/ 
static void MotoError(void) { 
    while(1) {
        MUSIC = 1;
        DelayMs(800);
        MUSIC = 0;
        DelayMs(800);
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoSet(u8 moto, u8 sleep) 
* �������: u8 moto, u8 sleep 
* ���ز���: void  
* ��    ��: �򿪵��
* ��    ��: by lhb_steven
* ��    ��: 2016/7/14
************************************************************************************************************/ 
u8 MotoOpen(u8 moto, u8 dir) { 
    //ѹֽ���
    if(moto == 0) {
        moto1.zero_time = 0;
        if(dir == 0) {
            //ѹֽ
            moto1.dir = ste_dr_pla_counter;
            moto1.pla_hd_time = 0;//ת��ʱ��
        } else {
            //�յ�
            moto1.dir = ste_dr_pla_positive;
        }
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//�رս����ж�ʹ��
        }
        PHASE_EN = 1;//����ת���ж�
        moto1.en = 1;//��ѹֽ���
        TIM2_CR1 = 0x01;
        //ѹֽ�������
        PLATEN_DIR = moto1.dir;
    } else {
        //������ֽ���
        moto2.zero_time = 0;
        if(dir == 0) {//��λ
            moto2.dir = ste_dr_cut_counter;
          
        } else {//��ֽ
            autu_n.specal_rst = 0;
            EepromWrite(13,autu_n.specal_rst);
            moto2.dir = ste_dr_cut_positive;
        }
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//�رս����ж�ʹ��
        }
        moto2.en = 1;//����ֽ���
        CUTTER_SLEEP = 0;//�͵�ƽ��Ч
        //��ֽ�������ʹ��
        CUTTER_DIR = moto2.dir;
    }
    //�����ж�
    //�ȴ��̵����ȶ�
    DelayMs(300);
    //ʹ�ܵ�Դ
    POWER_EN = 1;
    return 0;
}
/**********************************************��������***************************************************** 
* ��������: void MotoOff(u8 moto) 
* �������: u8 moto 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/18
************************************************************************************************************/ 
void MotoOff(u8 moto) { 
    if(moto == 0) {
        UART1_CR2_RIEN = 1;//�����ж�ʹ��
        //�رյ��
        moto1.en = 0;
        TIM2_CR1 = 0x00;//�رն�ʱ��
        PHASE_EN = 0;//�ر��ж�
        DelayMs(50);//500
        PLATEN_SLEEP = 1;//����
        //�����˹رռ̵���
        if(moto2.en == 0) { 
            POWER_EN = 0;//�رյ�Դ
        }
        DelayMs(50);//500
        PHASE_EN = 0;//�ر�ת���ж�
        DelayMs(400);//500
        PLATEN_DIR = 0;//�ͷŷ���̵���
        moto1.pla_hd_time = 0;//ת��
    } else if(moto == 1){
        UART1_CR2_RIEN = 1;//�����ж�ʹ��
        moto2.en = 0;
        CUTTER_SLEEP = 1;
        DelayMs(50);
        //�����˹رռ̵���
        POWER_EN = 0;//�رյ�Դ
        DelayMs(200);//500
        CUTTER_DIR = 0;//�ͷŷ���̵���
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoSetppingSet(u8 en, u8 dr,u8 set) 
* �������: u8 en, u8 dr,u8 set 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MotoSetppingSet(u8 mode , u8 dr, u16 arrive) { 
    if(mode == 0) {//�رյ��
        UART1_CR2_RIEN = 1;//�����ж�ʹ��
        TIM1_CR1 = 0x00;//������ʹ�ܣ���ʼ����
        stepping1.en = 0;
        stepping1.sleep = 0;//�����ٶ�
        EepromWrite(10,(u8)(stepping1.position));
        EepromWrite(11,(u8)(stepping1.position >> 8));
        ComSend(0x01,(u8)stepping1.position, (u8)(stepping1.position >> 8),0x00);//���͵�ǰλ��
    } else if(mode == 1){//�򿪵��
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//�رս����ж�ʹ��
        }
        if(dr == 0) {
            stepping1.dir = 0;
            STEPPING_DIR = 0;
        } else {
            stepping1.dir = 1;
            STEPPING_DIR = 1;
        }
        stepping1.arrive_en = 0;//����λ��
        stepping1.sleep = 0;//�����ٶ�
        stepping1.en = 1;//�������
        TIM1_CR1 = 0x01;//������ʹ�ܣ���ʼ����
    } else if(mode == 2){//�ߵ�ָ��λ��
        if(arrive <= ste_best_arrive) {
            if(arrive >= 400) {
                if(arrive != stepping1.position) {
                    if(arrive > stepping1.position) {
                        stepping1.dir = 1;
                        STEPPING_DIR = 1;
                    } else {
                        stepping1.dir = 0;
                        STEPPING_DIR = 0;
                    }
                    if(autu_n.specal_old == 0) {
                        UART1_CR2_RIEN = 0;//�رս����ж�ʹ��
                    }
                    stepping1.arrive_en = 1;//����λ��
                    stepping1.arrive_setp = arrive;
                    stepping1.sleep = 0;//�����ٶ�
                    stepping1.en = 1;//�������
                    TIM1_CR1 = 0x01;//������ʹ�ܣ���ʼ����
                }
            }
        } 
    } else {//У׼ģʽ
        autu_n.process = 13;
        autu_n.en = 1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoDivideSet(void) 
* �������: void 
* ���ز���: void  
* ��    �ܣ� 
* ��    ��: by lhb_steven
* ��    ��: 2016/8/20
************************************************************************************************************/ 
void MotoDivideSet(u8 num, u16 setp) {
    switch( num ) {
        case 0://�鱾����
            autu_n.sui_length_book = setp;
            EepromWrite(17,(u8)(autu_n.sui_length_book));
            EepromWrite(18,(u8)(autu_n.sui_length_book >> 8));
            break;
        case 1://ÿ�δ���
            autu_n.sui_length = setp;
            EepromWrite(15,(u8)(autu_n.sui_length));
            EepromWrite(16,(u8)(autu_n.sui_length >> 8));
            break;
        case 2://���м���
             autu_n.sui_cut_num = setp;
             EepromWrite(14,autu_n.sui_cut_num);
            break;
        default:
            break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MusicSet(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/8/12
************************************************************************************************************/ 
void MusicSet(void) {
	music1.open = 1;//�򿪷�����
	MUSIC = 1;//Ӳ����������
}

/**********************************************��������***************************************************** 
* ��������: void MotoServer(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �������  
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MotoServer(void) {
	//������������
//	if(music1.open == 1) {
//		if(music1.time < 10000) {
//			music1.time++;
//		} else {
//			music1.time = 0;
//			MUSIC = 0;
//		}
//	}
    //��ȫ��Ļ����
    if(SAFETY_LIGHT == 0) {
        if(moto1.en == 1) {
            //�رյ��
            //MotoOff(0);
			//��������
			MusicSet();
        }
        if(moto2.en == 1) {
            //�رյ��
            //MotoOff(1);
			//��������
			MusicSet();
        }
    }
    //�����������
    if(stepping1.en == 1) {
        //�����ߵķ���
        if(stepping1.dir == 1) {
            //�ص����
            if(STEPPING_ZERO == 0) {
                MotoSetppingSet(0,0,0);
                stepping1.position = 4900;
            }
        }
        if(stepping1.updata < 10000) {
            stepping1.updata++;
        } else {
            stepping1.updata = 0;
            ComSend(0x01,(u8)stepping1.position, (u8)(stepping1.position >> 8),0x00);//���͵�ǰλ��
        }
    }
    //ѹֽ�������
    if(moto1.en == 1) {//��ֽ�����
        if(moto1.dir == ste_dr_pla_counter) {
            //ѹֽ
            if(moto1.pla_hd_time < 4500) {
                moto1.pla_hd_time++;
            } else {
                moto1.pla_hd_time = 0;//�رյ��
                //�رյ��
                MotoOff(0);
            }
        } else {
            //��ֽ
            if(PLATEN_UP == 0) {
                //��������λ
                //�رյ��
                MotoOff(0);
            }
        }
        if(moto1.zero_time_l < 200) {
            moto1.zero_time_l++;
        } else {
            moto1.zero_time_l = 0;
            if(moto1.zero_time < std_pla_down_pro_time) {
                moto1.zero_time++;
            } else {
                //�رյ��
                MotoOff(0);//�ر�ѹֽֽ���
                if(moto1.dir == ste_dr_pla_counter) {
                    ComSend(0x44,0x00,0x00,0x00);//��ֽ����
                } else {
                    ComSend(0x44,0x01,0x00,0x00);//��ֽ����
                }
                MotoError();
            }
        }
    }
    //��ֽ�������
    if(moto2.en == 1) {
        if(moto2.dir == ste_dr_cut_positive) {
            //��ֽ
            if(CUTTER_DOWN == 0) {
                if(moto2.cut_down_flag == 0) {
                    moto2.cut_down_flag = 1;
                } else if(moto2.cut_down_flag == 2){
                    moto2.cut_down_flag = 0;
                     //�رյ��
                    MotoOff(1);
                    ComSend(0x06,0x00,0x00,0x00);//��ֽ����
                }
            } else {
                if(moto2.cut_down_flag == 1) {
                    moto2.cut_down_flag = 2;
                }
            }
        }
        if(moto2.zero_time_l < 200) {
            moto2.zero_time_l++;
        } else {
            moto2.zero_time_l = 0;
            if(moto2.zero_time < std_pla_up_pro_time) {
                moto2.zero_time++;
            } else {
                //�رյ��
                MotoOff(1);//�ر�ѹֽֽ���
                if(moto2.dir == ste_dr_cut_positive) {
                    ComSend(0x44,2,0x00,0x00);//��ֽ����
                } else {
                    ComSend(0x44,3,0x00,0x00);//��ֽ����
                }
                MotoError();
            }
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuAutuSer(void) 
* �������: void 
* ���ز���: void  
* ��    ��: //�Զ�ģʽ�ṹ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/27
************************************************************************************************************/ 
void MenuAutuSer(void) { 
    if(autu_n.en == 1) {
        switch( autu_n.process ) {
        case 0://ѹֽ
            MotoOpen(0,0);//ѹֽ
            autu_n.time = 0;
            autu_n.process = 1;
            break;
        case 1://�ȴ�ѹֽ���
            if(autu_n.time < 55000) {
                autu_n.time++;
            } else {
                autu_n.time = 0;
                autu_n.process = 3;
                //autu_n.specal_rst = 0;
                MotoOpen(1,1);//��ֽ
            }
            if(moto1.en == 0) {
                autu_n.time = 0;
                autu_n.process = 2;
            }
            break;
        case 2://��ֽ
            //autu_n.specal_rst = 0;
            autu_n.process = 3;
            MotoOpen(1,1);//��ֽ
            break;
        case 3://�ȴ���ֽ���
            if(moto2.en == 0) {
                autu_n.process = 4;
            }
            break;
        case 4://�ջ�ѹֽ
            autu_n.process = 5;
            MotoOpen(0,1);//�ջ�    
            break;
        case 5://�ȴ��ջ�ѹֽ
            if(moto1.en == 0) {
                if(autu_n.specal_old == 0) {//�����ϻ�ģʽ
                    if(autu_n.specal_sui == 0) {
                        if(autu_n.push_book == 0) {
                            //����
                            autu_n.en = 0;
                            autu_n.process = 0;
                        } else {
                            //������ֽ
                            autu_n.process = 6;
                        }
                    } else {
                        //����ȷ�λ��
                        if(autu_n.sui_cut_num_last < autu_n.sui_cut_num) {
                            autu_n.process = 17;
                        } else {
                            //����
                            autu_n.specal_sui = 0;//�ȷ�ģʽ����
                            autu_n.en = 0;
                            autu_n.process = 0;
                        }
                    }
                } else if(autu_n.specal_old == 1) {//��ֽ�ϻ�
                    autu_n.process = 0;//������ֽ
                } else if(autu_n.specal_old == 2) {//��ֽ��˿��
                    autu_n.process = 19;//˿��
                } else {
                    //����
                    autu_n.en = 0;
                    autu_n.process = 0;
                }
            }
            break;
        case 6://�ж��Ƿ���Ҫ��ֽ
            //��ֽλ����Ҫ����
            autu_n.process = 7;
            stepping1.position_last = stepping1.position;
            if( (stepping1.position-200) <= 400 ) {
                MotoSetppingSet(2,0,400);//��ֽ
            } else {
                MotoSetppingSet(2,0,stepping1.position-200);//��ֽ
            }
            break;
        case 7://�ȴ���ֽ���
            if(stepping1.en == 0) {
                autu_n.process = 8;
                MotoSetppingSet(2,0,stepping1.position_last);//��ֽ
            }
            break;
        case 8://�ȴ���ֽ���
            if(stepping1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*---------------------��λģʽ----------------------------------------------*/
        case 9://�յ�
            autu_n.process = 10;
            MotoOpen(1,0);
            break;
        case 10://�ȴ��յ����
            if(moto2.en == 0) {
                autu_n.process = 11;//�յ����
                autu_n.specal_rst = 1;
                EepromWrite(13,autu_n.specal_rst);
            }   
            break;
        case 11://�ջ�ѹֽ
            autu_n.process = 12;
            MotoOpen(0,1);//�ջ�   
            break;
        case 12://�ȴ��ջ�ѹֽ���
            if(moto1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*-------------------�������У׼--------------------------------------------*/
        case 13://�ߵ���λλ��
            autu_n.process = 14;
            MotoSetppingSet(1,1,0);
            break;
        case 14://�ȴ���λ���
            if(stepping1.en == 0) {
                autu_n.process = 15;
            }
            break;
        case 15://�ߵ��ϴε�λ��
            autu_n.process = 16;
            MotoSetppingSet(2,0,stepping1.position_last);
            break;
        case 16://�ȴ���λ�����
            if(stepping1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*--------------�ȷ�ģʽ------------------------------------------------------*/
        case 17://�ߵ�λ��
            autu_n.process = 18;
            if(autu_n.sui_cut_num_last < autu_n.sui_cut_num) {
                autu_n.sui_cut_num_last++;
            }
            MotoSetppingSet(2,0,autu_n.sui_length_book - (autu_n.sui_cut_num_last*autu_n.sui_length));
            break;
        case 18://�ȴ���λ�����
            if(stepping1.en == 0) {
                autu_n.process = 0;//��ʼѹֽ
            }
            break;
/*------------�ϻ�ģʽ--------------------------------------------------------*/
        case 19://�ߵ�ԭ��
            autu_n.process = 20;
            MotoSetppingSet(1,1,0);
            break;
        case 20://�ȴ������������
            if(stepping1.en == 0) {
                autu_n.process = 21;
            }
            break;
        case 21://�ߵ���ʼλ��
            autu_n.process = 22;
            MotoSetppingSet(2,0,400);
            break;
        case 22://�ȴ�����
            if(stepping1.en == 0) {
                autu_n.process = 19;
                if(autu_n.specal_old == 2) {//��ֽ��˿���ϻ�
                    autu_n.process = 0;
                } else if(autu_n.specal_old == 3) {//˿���ϻ�
                    autu_n.process = 19;
                } else {
                    autu_n.en = 0;
                    autu_n.process = 0;
                }
            }
            break;
        default:
            break;
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: �򿪻�ر� MotoOldMode(u8 cmd,u8 mode) 
* �������: u8 cmd,u8 mode 
* ���ز���: �򿪻�ر�  
* ��    ��: �ϻ�ģʽ  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/29
************************************************************************************************************/ 
void MotoOldMode(u8 cmd,u8 mode) { 
    if(cmd == 0) {
        autu_n.specal_old = 0;
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 2:
            MotoOff(1);//�ر���ֽֽ���
            break;
        case 3:
            MotoOff(1);//�ر���ֽֽ���
            if(moto1.en == 1) {//˵��ѹֽû��ɵ���Ͷ���
                MotoOff(0);//�ر�ѹֽ���
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 5:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 6://�ж��Ƿ���Ҫ��ֽ
            MotoSetppingSet(0,0,0);
            break;
        case 20://�ȴ���ֽ���
            MotoSetppingSet(0,0,0);
            break;
        case 22://�ȴ���ֽ���
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.process = 0;
        autu_n.time = 0;
        autu_n.en = 0;
    } else { 
        if(autu_n.en == 0) {
            switch( mode ) {
                case 0://��ֽ�ϻ�
                    autu_n.specal_old = 1;
                    autu_n.process = 0;
                    break;
                case 1://��ֽ��˿���ϻ�
                    autu_n.specal_old = 2;
                    autu_n.process = 0;
                    break;
                case 2://˿���ϻ�
                    autu_n.specal_old = 3;
                    autu_n.process = 19;
                    break;
                case 3:
                    
                    break;
                default:
                    break;
            }
            autu_n.en = 1;
        } else {
            MotoOldMode(0,0);
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuAutuSui(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: ��ʼ�ȷ�ģʽ  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/28
************************************************************************************************************/ 
void MenuAutuSui(u8 cmd) { 
    if(cmd == 0) {
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 2:
            MotoOff(1);//�ر���ֽֽ���
            break;
        case 3:
            MotoOff(1);//�ر���ֽֽ���
            if(moto1.en == 1) {//˵��ѹֽû��ɵ���Ͷ���
                MotoOff(0);//�ر�ѹֽ���
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 5:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 17:
            MotoSetppingSet(0,0,0);
            break;
        case 18://�ȴ���λ�����
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.time = 0;
        autu_n.en = 0;
    } else {
        if(autu_n.specal_sui == 0) {
            autu_n.process = 17;
            autu_n.sui_cut_num_last = 0;
            autu_n.specal_sui = 1;//�ȷ�ģʽ
        }
        switch( autu_n.process ) {
        case 0:
            break;
        case 1:
            MotoOpen(0,0);//��ѹֽ
            break;
        case 2:
            break;
        case 3:
            if(autu_n.specal == 0) {
                MotoOpen(1,1);//����ֽ
            } else {
                //ѹֽû�����ֽ��������
                autu_n.specal = 0;
                autu_n.process = 0;
            }
            break;
        case 4:
            break;
        case 5:
            MotoOpen(0,1);//�ջ�ѹֽ���  
            break;
        case 17:
            break;
        case 18://�ȴ���λ�����
            MotoSetppingSet(2,0,autu_n.sui_length_book - (autu_n.sui_cut_num_last*autu_n.sui_length));
            break;
        default:
            break;
        }
        autu_n.en = 1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuAutoSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: ����ģʽ  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/27
************************************************************************************************************/ 
void MenuAutoSet(u8 cmd) { 
    if(cmd == 0) {
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 2:
            MotoOff(1);//�ر���ֽֽ���
            break;
        case 3:
            MotoOff(1);//�ر���ֽֽ���
            if(moto1.en == 1) {//˵��ѹֽû��ɵ���Ͷ���
                MotoOff(0);//�ر�ѹֽ���
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 5:
            MotoOff(0);//�ر�ѹֽ���
            break;
        case 6://�ж��Ƿ���Ҫ��ֽ
            MotoSetppingSet(0,0,0);
            break;
        case 7://�ȴ���ֽ���
            MotoSetppingSet(0,0,0);
            break;
        case 8://�ȴ���ֽ���
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.time = 0;
        autu_n.en = 0;
    } else {
        autu_n.specal_sui = 0;//�رյȷ�ģʽ
        switch( autu_n.process ) {
        case 0:
            break;
        case 1:
            MotoOpen(0,0);//��ѹֽ
            break;
        case 2:
            break;
        case 3:
            if(autu_n.specal == 0) {
                MotoOpen(1,1);//����ֽ
            } else {
                //ѹֽû�����ֽ��������
                autu_n.specal = 0;
                autu_n.process = 0;
            }
            break;
        case 4:
            break;
        case 5:
            MotoOpen(0,1);//�ջ�ѹֽ���  
            break;
        case 6://�ж��Ƿ���Ҫ��ֽ
            break;
        case 7://�ȴ���ֽ���
            if( (stepping1.position-200) <= 400 ) {
                MotoSetppingSet(2,0,400);//��ֽ
            } else {
                MotoSetppingSet(2,0,stepping1.position-200);//��ֽ
            }
            break;
        case 8://�ȴ���ֽ���
            MotoSetppingSet(2,0,stepping1.position_last);//��ֽ
            break;
        default:
            break;
        }
        autu_n.en = 1;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoReset(void) 
* �������: void
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/8/15
************************************************************************************************************/ 
void MotoReset(void) {
    if(autu_n.specal_rst == 1) {
        autu_n.process = 11;
    } else {
        autu_n.process = 9;
    }
    autu_n.en = 1;
}
/**********************************************��������***************************************************** 
* ��������: void MotoAutuPushBook(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: �Ƿ���Ҫ�Զ���������  
* ��    ��: by lhb_steven
* ��    ��: 2016/9/28
************************************************************************************************************/ 
void MotoAutuPushBook(u8 cmd) { 
    autu_n.push_book = cmd;
    EepromWrite(12,autu_n.push_book);
}
/**********************************************��������***************************************************** 
* ��������: void MotoSetStrength(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/9/29
************************************************************************************************************/ 
void MotoSetStrength(u8 cmd) { 
    switch( cmd ) {
        case 1:
            moto1.strength = 4000;
            break;
        case 2:
            moto1.strength = 3000;
            break;
        case 3:
            moto1.strength = 2000;
            break;
        case 4:
            moto1.strength = 1000;
            break;
        default:
            moto1.strength = 1000;
            break;
    }
    EepromWrite(19,(u8)(moto1.strength));
    EepromWrite(20,(u8)(moto1.strength >> 8));
}
/*
	��ʱ���жϣ�����
*/
long xx = 0;
static const u16 setp_sleep[10] = {800,700,600,500,400,300,250,200,160,130};
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//����жϱ�־
    if(stepping1.dr == 0) {
        stepping1.dr = 1;
        STEPPING_SLEEP = 1;
    } else {
        stepping1.dr = 0;
        STEPPING_SLEEP = 0;
        
    }
    if(stepping1.setp < 19) {//ÿ20������0.1mm
        stepping1.setp++;
    } else {
        stepping1.setp = 0;
        if(stepping1.dir == 1) {
            if(stepping1.position < 4900) {//�������� 490-40 = 450mm
                stepping1.position++;
            } else {
                
            }
            if(stepping1.protection < 5000) {
                stepping1.protection++;
            } else {
                //����������
                ComSend(0x44,4,0,0);
                MotoSetppingSet(0,0,0);
                MotoError();
            }
        } else {
            stepping1.protection = 0;
            if(stepping1.position > 400) {//�������� 490-40 = 450mm
                stepping1.position--;
            } else {
                //�رյ��
                stepping1.en = 0;
            }
        }
        //��λ��
        if(stepping1.arrive_en == 1) {//����λ��
            if(stepping1.arrive_setp == stepping1.position) {
                //�رյ��
                stepping1.en = 0;
                stepping1.arrive_en = 0;
            }
        }
        //�رյ��
        if(stepping1.en == 0) {
            //TIM1_CR1 = 0x00;//������ʹ�ܣ���ʼ����
            MotoSetppingSet(0,0,0);
        }
    }
    //���ٶ�����
    if(stepping1.sleep < 9) {
        static u8 stp_time = 0;
        if(stp_time < 100) {
            stp_time++;
        } else {
            stp_time = 0;
            TIM1_ARRH = (u8)(setp_sleep[stepping1.sleep] >> 8);
            TIM1_ARRL = (u8)(setp_sleep[stepping1.sleep]);//ÿ500us�ж�һ��
            stepping1.sleep++;
        }
    }
    INTEN
}


#pragma   vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_OVR_UIF(void)
{
    TIM2_SR1_UIF = 0;
    //add your code here
    if(moto1.sleep_start == 1) {
        switch( moto1.flag ) {
            case 0:
                moto1.flag = 1;
                PLATEN_SLEEP = 0;
                TIM2_ARRH = 0x3;
                TIM2_ARRL = 0xE8;//ÿ1000us�ж�һ��
                break;
            case 1:
                PLATEN_SLEEP = 1;
                moto1.flag = 0;
                moto1.sleep_start = 0;
                break;
            default:
            
            break;
        }
    }
}


#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  
}

#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void) {
    INTOFF
    //���յ�50HZ ��Ƶ�ź�
    if(moto1.en == 1) {
        moto1.sleep_start = 1;
        TIM2_CR1 = 0x00;//������ʹ�ܣ���ʼ����
        TIM2_ARRH = (u8)(moto1.strength >> 8);
        TIM2_ARRL = (u8)moto1.strength;        //ÿ4000us�ж�һ��
        TIM2_CR1 = 0x01;//������ʹ�ܣ���ʼ����
    }
    INTEN
    return;                    
}

#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void) {
    INTOFF
    if(moto1.en == 1) {//��ֽ�����
        moto1.pla_hd_time = 0;
    }
    //��λλ��
    if(CUTTER_UP == 0) {
        if(moto2.en == 1) {
            if(moto2.dir == ste_dr_pla_positive) {
                //�رյ��
                MotoOff(1);
            }
        }
    }
    INTEN
    return;                    
}

