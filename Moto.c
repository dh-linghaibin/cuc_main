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
/*�Զ���־λ*/
static u8 autu_flag = 0;
/*�ϻ�ģʽ��־λ*/
static u8 age_mode_flag = 0;//�ϻ�ģʽ��־λ  0������ 1�������ϻ�ģʽ
static u8 age_mode_num = 0;//�ϻ�ģʽ��־λ  0������ 1�������ϻ�ģʽ
static u8 age_mode_dr = 0;//�ϻ�ģʽ��־λ  0������ 1�������ϻ�ģʽ
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
    PB_CR1_C12 = 0;
    PB_CR2_C22 = 1;
    
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
    
    PC_DDR_DDR3 = 1;//������ʹ��
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    PB_DDR_DDR5 = 0;//��ȫ��Ļ
    PB_CR1_C15 = 0;
    PB_CR2_C25 = 0;
    
    CUTTER_SLEEP = 1;
    PLATEN_SLEEP = 1;
    
    //��ʱ��1���ã�����1msһ�ε��ж�
	TIM1_PSCRH = 0X00;
	TIM1_PSCRL = 0Xc7;
	TIM1_ARRH = 0X00;
	TIM1_ARRL = 0X05;//0a
	TIM1_IER = 0X01;
	TIM1_CR1 = 0X01;
    
    CUTTER_LIGHT = 1;//�򿪼���ָʾ
    
    EXTI_CR1 &= ~BIT(2);//����PD���ж� 1 0
	EXTI_CR1 |= BIT(3);
    
    //��һ�γ�ʼ��
    if(EepromRead(9) != 0x55) {
        EepromWrite(9,0x55);
        EepromWrite(10,0x00);
        EepromWrite(11,0x00);
        //���ȳ�ʼ��
        EepromWrite(12,0x04);
        EepromWrite(13,0x04);
        
        EepromWrite(20,0x04);
    }
    stepping1.step = EepromRead(10);
    stepping1.step |= (u16)(EepromRead(11) << 8);
    
    //���������ʼ��
    moto1.sleep_set_group = EepromRead(12);//4
    moto1.sleep_set = moto1.sleep_set_group*10;
    moto1.sleep = moto1.sleep_set;
    
    moto2.sleep_set_group = EepromRead(13);//4
    moto2.sleep_set = moto2.sleep_set_group*10;
    moto2.sleep = moto1.sleep_set;
    
    //��ȡ��ʼλ��
    moto2.cut_zero_time_l = EepromRead(20);
    moto2.cut_zero_time = moto2.cut_zero_time_l*6666;
    
    stepping1.start_zero = 0;//��һ�ο���
}
/**********************************************��������***************************************************** 
* ��������: static void MotoSetDyn(u8 moto,u8 dyn) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/8/15
************************************************************************************************************/ 
u8 MotoSetDyn(u8 moto,u8 dyn) {
    if(dyn == 0) {
        return 0x00;
    } else if(dyn > 10) {
        return 0x00;
    }
    if(moto == 0) {
        moto1.sleep_set_group = dyn;//4
        moto1.sleep_set = moto1.sleep_set_group*10;
        moto1.sleep = moto1.sleep_set;
        EepromWrite(12,dyn);
    } else if(moto == 1){
        moto2.sleep_set_group = dyn;//4
        moto2.sleep_set = moto2.sleep_set_group*10;
        moto2.sleep = moto1.sleep_set;
        EepromWrite(13,dyn);
    } else if(moto == 2){
        //��ȡ��ʼλ��
        moto2.cut_zero_time_l = EepromRead(20);
        moto2.cut_zero_time = moto2.cut_zero_time_l*6666;
        EepromWrite(20,dyn);
    }
    return 0x00;
}
/**********************************************��������***************************************************** 
* ��������: static void MotoSaveStep(void) 
* �������: void 
* ���ز���: static void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
static void MotoSaveStep(void) { 
    EepromWrite(10,(u8)(stepping1.step));
    EepromWrite(11,(u8)(stepping1.step >> 8));
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
        //�رյ��
        moto1.en = 0;
        PLATEN_SLEEP = 1;
        moto1.down_time = 0;
        moto1.up_time = 0;
        //�����˹رռ̵���
        POWER_EN = 0;//�رյ�Դ
        DelayMs(500);
        PLATEN_DIR = 0;//�ͷŷ���̵���
    } else {
        moto2.en = 0;
        CUTTER_SLEEP = 1;
        moto2.down_time = 0;
        moto2.up_time = 0;
        moto2.cut_down_flag = 0;
        //�����˹رռ̵���
        POWER_EN = 0;//�رյ�Դ
        DelayMs(500);
        CUTTER_DIR = 0;//�ͷŷ���̵���
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoSet(u8 moto, u8 sleep) 
* �������: u8 moto, u8 sleep 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/14
************************************************************************************************************/ 
u8 MotoSet(u8 moto, u8 sleep, u8 dir) { 
    if(moto == 0) {
        if(moto2.en == 1) {//��ֻ�������ͬʱ��ת
            return 0x00;
        }
        if(moto1.en == 1) {//�ڴ򿪵�����²����л�
//            //�رյ��
            MotoOff(0);
            return 0x00;
        }
        CUTTER_SLEEP = 1;
        PLATEN_SLEEP = 1;
        moto1.en = 1;//��ѹֽ���
        if(dir == 0) {
            moto1.dir = ste_dr_cut_counter;
            moto1.down_time = 0;
            if(PLATEN_UP == 0) {
                return 0;
            }
        } else {
            moto1.dir = ste_dr_cut_positive;
            moto1.up_time = 0;
        }
        //ѹֽ�������
        PLATEN_DIR = moto1.dir;
    } else {
        moto2.cut_zero_time = 0;
        moto2.cut_zero_time_l = 0;
        moto2.cut_down_flag = 0;
        if(moto1.en == 1) {//��ֻ�������ͬʱ��ת
            return 0x00;
        }
        if(moto2.en == 1) {//�ڴ򿪵�����²����л�
            MotoOff(1);
            return 0x00;
        }
        PLATEN_SLEEP = 1;
        CUTTER_SLEEP = 1;
        moto2.en = 1;//����ֽ���
        if(dir == 0) {
            moto2.dir = ste_dr_pla_counter;
            moto2.down_time = 0;
            if(CUTTER_UP == 0) {
                return 0;
            }
        } else {
            moto2.dir = ste_dr_pla_positive;
            moto2.up_time = 0;
            if(CUTTER_DOWN == 1) {//�޸�1��λ�� ����
                //return 0;
            }
        }
        //��ֽ�������ʹ��
        CUTTER_DIR = moto2.dir;
    }
    //�����ж�
    PHASE_EN = 1;
    //�ȴ��̵����ȶ�
    DelayMs(300);
    //ʹ�ܵ�Դ
    POWER_EN = 1;
    return 0;
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
    //��ֽ�������λ��
//    if(CUTTER_UP == 1) {
//        //�ջ���ֽ���
//        MotoSet(1,10,1);
//    } 
//    //ѹֽ�������λ��
//    if(CUTTER_UP == 1) {
//        //�ջ�ѹֽ���
//        MotoSet(0,10,1);
//    }
    //�ȼ�⵶�ڲ���λ��  �ٽ��ż쳵ѹֽ�ڲ���λ�� 
    if( (CUTTER_DOWN == 0) && (PLATEN_UP == 0) ){//( (CUTTER_UP CUTTER_DOWN== 0) && (PLATEN_UP == 0) ){//�ӽ����ز��� �ֳ������޸�
         //��ʼ�Զ����� ��ѹֽ
    } else {
        //���жϵ��ڲ���λ��
        if(CUTTER_UP == 1) {
            //������λ�ã�����
            MotoSet(1,10,0);
            autu_flag = 20;
        } else if(PLATEN_UP == 1) {
            //����λ�ã�ѹֽ����λ��
            MotoSet(0,10,0);
            autu_flag = 22;//����
        }
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
void MotoSetppingSet(u8 en, u8 dr,u8 set) { 
    if(en == 0) {
        stepping1.en = 0;
        MotoSaveStep();//����
        ComSend(0x07,(u8)stepping1.step, (u8)(stepping1.step >> 8),0x00);//���͵�ǰλ��
    } else {
        if(dr == 0) {
            stepping1.dir = 0;
            STEPPING_DIR = 0;
        } else {
            stepping1.dir = 1;
            STEPPING_DIR = 1;
        }
        stepping1.sleep = 30;//�����ٶ�
        stepping1.en = 1;//�������
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoSteppingSetp(u8 mode, u8 arrive) 
* �������: u8 mode, u8 arrive 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
************************************************************************************************************/ 
extern tDataNode menu;
void MotoSteppingSetp(u8 mode, u16 arrive) { 
    if(mode == 0) {//��λ
        stepping1.dir = 1;
        STEPPING_DIR = 1;
        stepping1.sleep = 30;//�����ٶ�
        stepping1.en = 1;//�������
    } else if(mode == 1) {//����
        stepping1.dir = 0;
        STEPPING_DIR = 0;
        stepping1.sleep = 30;//�����ٶ�
        stepping1.en = 1;//�������
    } else if(mode == 2) {
        if(arrive <= ste_best_arrive) {
            if(arrive == stepping1.step) {
            
            } else {
                if(arrive > stepping1.step) {
                    stepping1.dir = 0;
                    STEPPING_DIR = 0;
                } else {
                    stepping1.dir = 1;
                    STEPPING_DIR = 1;
                }
                stepping1.arrive_setp = arrive;
                stepping1.arrive_en = 1;//����λ��
                stepping1.sleep = 30;//�����ٶ�
                stepping1.en = 1;//�������
            }
        } 
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
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/15
************************************************************************************************************/ 
void MotoServer(void) {
	//������������
	if(music1.open == 1) {
		if(music1.time < 10000) {
			music1.time++;
		} else {
			music1.time = 0;
			MUSIC = 0;
		}
	}
    //��ȫ��Ļ����
    if(SAFETY_LIGHT == 0) {
        if(moto1.en == 1) {
            //�رյ��
            MotoOff(0);
			//��������
			MusicSet();
        }
        if(moto2.en == 1) {
            //�رյ��
            MotoOff(1);
			//��������
			MusicSet();
        }
    }
    //�����
    if(stepping1.en == 1) {
        //�����������
        if(stepping1.dir == 1) {
            //�ص����
            if(STEPPING_ZERO == 0) {
                stepping1.en = 0;//�رյ��
                stepping1.step = 0;//�ص����
                MotoSaveStep();//����
                //��һ������ ��Ҫ�ص�λ��
                //������ɣ��ߵ�λ��
                if(stepping1.start_zero == 0) {
                    stepping1.start_zero = 1;//ֻ���ڿ�����ʱ����һ��
					MusicSet();//��λ��� ��һ��
                    MotoSteppingSetp(2,MenuGetArrive());
                }
                if(autu_flag == 60) {//�ϻ�ģʽ
                    autu_flag = 1;
                }
				//�ص���λ�����͵��λ��
				ComSend(0x07,(u8)stepping1.step, (u8)(stepping1.step >> 8),0x00);//���͵�ǰλ��
            }
#if MOTO_PRP == 1
            if(stepping1.zero_time < mot_zero_time) {
                static u8 cut_up_count = 0;
                if(cut_up_count < 10) {
                    cut_up_count++;
                } else {
                    cut_up_count = 0;
                    stepping1.zero_time++;
                }
            } else {
                stepping1.zero_time = 0;
                //�رյ��
                stepping1.en = 0;//�رյ��
                stepping1.step = 0;//�ص����
                MotoSaveStep();//����
                //���ʹ���
                MusicSet();//�������󣬽�һ��
				ComSend(0x44,0x03,0x00,0x00);
            }
#endif
        }
		//���λ�ø���
		{
			static u16 setp_time = 0;
			if(setp_time < 2000) {
				setp_time++;
			} else {
				setp_time = 0;
				//����λ��
				ComSend(0x07,(u8)stepping1.step, (u8)(stepping1.step >> 8),0x00);//���͵�ǰλ��
			}
		}
    }
    //ѹֽ����Դ�
    if(moto1.en == 1) {
        if(moto1.dir == ste_dr_cut_counter) {
            //ѹֽ�������
#if MOTO_PRP == 1
            if(moto1.down_time < std_cut_down_pro_time) {
                static u8 cut_up_count = 0;
                if(cut_up_count < 10) {
                    cut_up_count++;
                } else {
                    cut_up_count = 0;
                    moto1.down_time++;
                }
            } else {
                moto1.up_time = 0;
                //�رյ��
                MotoOff(0);
                //���ʹ���
                MusicSet();//�������󣬽�һ��
				ComSend(0x44,0x01,0x00,0x00);
            }
#endif
            //��λ����
            if(PLATEN_UP == 0) {
                //�رյ��
                MotoOff(0);
                //�Զ�����
                if(autu_flag == 8) {
                    autu_flag = 9;
                }
                //�յ�
                if(autu_flag == 22) {
                    if(age_mode_flag == 0) {//���ϻ�ģʽ����
                        autu_flag = 0;
                    } else {//�ϻ�ģʽ��������
                        if(age_mode_num < 2) {
                            age_mode_num++;
                            autu_flag = 1;
                        } else {
                            age_mode_num = 0;
                            if(age_mode_dr == 0) {
                                age_mode_dr = 1;
                                MotoSteppingSetp(2,ste_best_arrive);
                            } else {
                                age_mode_dr = 0;
                                MotoSteppingSetp(2,0x00);
                            }
                            autu_flag = 60;
                        }
                    }
                }
            }
        } else {
            //ѹֽ�������
#if MOTO_PRP == 1
            if(moto1.down_time < std_cut_down_pro_time) {
                static u8 cut_up_count = 0;
                if(cut_up_count < 10) {
                    cut_up_count++;
                } else {
                    cut_up_count = 0;
                    moto1.down_time++;
                }
            } else {
                moto1.up_time = 0;
                //�رյ��
                MotoOff(0);
                //���ʹ���
                MusicSet();//�������󣬽�һ��
				ComSend(0x44,0x02,0x01,0x00);
            }
#endif
            static u16 hd_count1 = 0;
            if(moto1.yazhi_jb > 1) {
                moto1.yazhi_jb = 0;
                hd_count1 = 0;
            } else {
                PB_CR2_C22 = 1;
                if(hd_count1 < 4000) {
                    hd_count1++;
                } else {
                    hd_count1 = 0;
                    MotoOff(0);
                    //�Զ�����
                    if(autu_flag == 2) {
                        autu_flag = 3;
                    }
                }
            }
        }
    }
    //��ֽ����Դ�
    if(moto2.en == 1) {
        if(moto2.dir == ste_dr_pla_counter) {//����-�ջص�--ֻ���ֶ�������
            //static u8 open_flag = 0;
            //static u8 open_van_out = 0;
            //static u8 open_van_in = 0;
            //��ֽ�������
#if MOTO_PRP == 1
            if(moto2.down_time < std_pla_down_pro_time) {
                static u8 pla_down_count = 0;
                if(pla_down_count < 10) {
                    pla_down_count++;
                } else {
                    pla_down_count = 0;
                    moto2.down_time++;
                }
            } else {
                moto2.down_time = 0;
                //�رյ��
                MotoOff(1);
                //���ʹ���
                MusicSet();//�������󣬽�һ��
				ComSend(0x44,0x02,0x00,0x00);
            }
#endif
            if(CUTTER_UP == 0) {//����������ź��ˣ��϶�Ҫֹͣ ����ź�ֹͣʱ���Ե� 
                if( moto2.cut_zero_time_l == 1) {
                    moto2.cut_zero_time_l = 0;
                    //�رյ��
                    MotoOff(1);//�ղ��Բ���
    //                if(autu_flag == 6) {
    //                    autu_flag = 7;
    //                }
    //                //��λ
                    if(autu_flag == 20) {
                        autu_flag = 21;
                    }
                }
            } else {
                if(moto2.cut_zero_time < 200) {
                    moto2.cut_zero_time++;
                } else {
                    moto2.cut_zero_time = 0;
                    moto2.cut_zero_time_l = 1;
                }
            }
            //�¼�
            if(CUTTER_DOWN == 0) {
                if(moto2.cut_down_flag == 0) {
                    moto2.cut_down_flag = 1;
                } else if(moto2.cut_down_flag == 2){
                    moto2.cut_down_flag = 0;
                    //�ر�
                     //�رյ��
                    MotoOff(1);
                    //�Զ�����
                    if(autu_flag == 20) {
                        autu_flag = 21;
                    }
                }
            } else {
                if(moto2.cut_down_flag == 1) {
                    moto2.cut_down_flag = 2;
                }
            }
            /*
            if(CUTTER_DOWN == 1) {
                if(open_flag == 0) {
                    if(open_van_out < 200) {
                        open_van_out++;
                    } else {
                        open_van_out = 0;
                        open_flag= 1;
                    }
                } else {
                    open_van_in = 0;//����
                }
            } else {
                if(open_flag == 1) {
                    if(open_van_in < 200) {
                        open_van_in++;
                    } else {
                        open_van_in = 0;
                        //�źŵ���
                        open_flag = 2;
                        //�����Ϣ
                        moto2.close_time = 0;
                    }
                } else {
                    open_van_out = 0;//����
                }
            }
            //�ر�
            if(open_flag == 2) {
                if(moto2.close_time < moto2.cut_zero_time) {
                    moto2.close_time++;
                } else {
                    moto2.close_time = 0;
                    //�رյ��
                    MotoOff(1); 
                    if(autu_flag == 20) {
                        autu_flag = 21;
                    }
                }
            }*/
        } else {//����--�µ�
            //��ֽ�������
#if MOTO_PRP == 1
            if(moto2.up_time < std_pla_up_pro_time) {
                static u8 pla_up_count = 0;
                if(pla_up_count < 10) {
                    pla_up_count++;
                } else {
                    pla_up_count = 0;
                    moto2.up_time++;
                }
            } else {
                moto2.up_time = 0;
                MotoOff(1);
                //���ʹ���
                MusicSet();//�������󣬽�һ��
				ComSend(0x44,0x02,0x00,0x00);
            }
#endif
            //�ֶ�û���µ�һ˵
            if(CUTTER_UP == 0) {
                if( moto2.cut_zero_time_l == 1) {
                    moto2.cut_zero_time_l = 0;
                    //�رյ��
                    MotoOff(1);
                    //�Զ�����
                    if(autu_flag == 4) {
                        autu_flag = 5;
                    }
                    //�ֶ���ֽ
                    if(autu_flag == 31) {
                        autu_flag = 32;
                    }
                }
            } else {
                if(moto2.cut_zero_time < 200) {
                    moto2.cut_zero_time++;
                } else {
                    moto2.cut_zero_time = 0;
                    moto2.cut_zero_time_l = 1;
                }
            }
             //�¼�
            if(CUTTER_DOWN == 0) {
                if(moto2.cut_down_flag == 0) {
                    moto2.cut_down_flag = 1;
                } else if(moto2.cut_down_flag == 2){
                    moto2.cut_down_flag = 0;
                    //�ر�
                     //�رյ��
                    MotoOff(1);
                    //�Զ�����
                    if(autu_flag == 4) {
                        autu_flag = 5;
                    }
                    //�ֶ���ֽ
                    if(autu_flag == 31) {
                        autu_flag = 32;
                    }
                }
            } else {
                if(moto2.cut_down_flag == 1) {
                    moto2.cut_down_flag = 2;
                }
            }
            //�µķ�ʽ
            /*
            static u8 close_flag = 0;
            static u8 close_van_out = 0;
            static u8 close_van_in = 0;
            if(CUTTER_DOWN == 0) {
                if(close_flag == 0) {
                    if(close_van_out < 200) {
                        close_van_out++;
                    } else {
                        close_van_out = 0;
                        close_flag= 1;
                    }
                } else {
                    close_van_in = 0;//����
                }
            } else {
                if(close_flag == 1) {
                    if(close_van_in < 200) {
                        close_van_in++;
                    } else {
                        close_van_in = 0;
                        //�źŵ���
                        close_flag = 2;
                        //�����Ϣ
                        moto2.close_time = 0;
                    }
                } else {
                    close_van_out = 0;//����
                }
            }
            if(close_flag == 2) {
                if(moto2.close_time < moto2.cut_zero_time) {
                    moto2.close_time++;
                } else {
                    //��ֽ���
                    if(age_mode_flag == 0) {//�ϻ�ģʽ������
                        ComSend(0x03,0x00, 0x00,0x00);//������ֽ�źţ�����
                    }
                    moto2.close_time = 0;
                    //�رյ��
                    MotoOff(1);
                    //�Զ�����-�޸�������
                    if(autu_flag == 4) {
                        autu_flag = 5;
                    }
                    //�ֶ���ֽ
                    if(autu_flag == 31) {
                        autu_flag = 32;
                    }
                }
            }*/
        }
    }
    //�Զ�����
    switch( autu_flag ) {
    case 1:
        //�ȼ�⵶�ڲ���λ��  �ٽ��ż쳵ѹֽ�ڲ���λ�� 
        if( (CUTTER_UP == 0) && (PLATEN_UP == 0) ) {//( (CUTTER_UP CUTTER_DOWN== 0) && (PLATEN_UP == 0) ){//�ӽ����ز��� �ֳ������޸�
             //��ʼ�Զ����� ��ѹֽ
            MotoSet(0,10,1);
            autu_flag = 2;
        } else {
            //���жϵ��ڲ���λ��
            if(CUTTER_UP == 1) {
                //������λ�ã�����
                MotoSet(1,10,0);
                autu_flag = 20;
            } else if(PLATEN_UP == 1) {
                //����λ�ã�ѹֽ����λ��
                MotoSet(0,10,0);
                autu_flag = 22;//����
            }
        }
        break;
    case 2:
        break;
    case 3:
        //ѹֽ��ɣ���ʼ��ֽ
        MotoSet(1,10,1);
        autu_flag = 4;
        break;
    case 4:
        break;
    case 5:
        //��ֽ���,�ѵ��ջ�
        //MotoSet(1,10,0); ���Ѿ��ջ��ˣ�����Ҫ���ջ�
        autu_flag = 7;
        break;
    case 6:
        break;
    case 7:
        //���Ѿ��ջ�,�ջ�ѹֽ
        MotoSet(0,10,0);
        autu_flag = 8;
        break;
    case 9:
        if(stepping1.divide_sec == 2) {//�ȷ�ģʽ
            autu_flag = 40;//�����ȷ�
        } else {
            if(age_mode_flag == 0) {//��ͨģʽ��������
                //ѹֽ�Ѿ��ջأ�����
                if(MenuGetPushBook() == 0) {
                    autu_flag = 0;//����
                } else {
                    //��ֽstepping1.step
                    //MotoSteppingSetp(1,0);
                    MotoSteppingSetp(2,menu.ste_arrive+2000);//ֻҪ��һ�������Ϳ�����
                    autu_flag = 10;
                }
            } else {//�ϻ�ģʽ��������
                //autu_flag = 1;
                if(age_mode_num < 2) {
                    age_mode_num++;
                    autu_flag = 1;
                } else {
                    age_mode_num = 0;
                    if(age_mode_dr == 0) {
                        age_mode_dr = 1;
                        MotoSteppingSetp(2,ste_best_arrive);
                    } else {
                        age_mode_dr = 0;
                        MotoSteppingSetp(2,0x00);
                    }
                    autu_flag = 60;
                }
            }
        }
        break;
    case 11:
        //������ɣ��ߵ�λ��
        MotoSteppingSetp(2,MenuGetArrive());
        autu_flag = 0;
        break;
    case 21:
        //���Ѿ��ջأ��ж�ѹֽ���
        if(PLATEN_UP == 1) {
            //����λ�ã�ѹֽ����λ��
            MotoSet(0,10,0);
            autu_flag = 22;//����
        } else {
            autu_flag = 0;//����
        }
        break;
    case 30://�ֶ�ģʽ ��ֽ
        if(CUTTER_DOWN == 0) {
            //��λ����ֽ
            MotoSet(1,10,1);
            autu_flag = 31;
        } else {
            autu_flag = 32;
        }
        break;
    case 32:
        //����λ�ø�λ
        //MotoSet(1,10,0);���Ѿ��ջأ�����Ҫ���յ�
        autu_flag = 0;
        break;
    case 40://�ȷ�ģʽ        ��ֽ���ϰ�ť ��Ҫ�ȼ��
        if(stepping1.divide_sec == 1) {
            stepping1.divide_num_s = 1;
            if(stepping1.divide_num > 0) {
                MotoSteppingSetp(2,(stepping1.divide_step*stepping1.divide_num)+(stepping1.divide_step*stepping1.divide_num_s));//��һ�ε�λ��
                //stepping1.divide_num--;
                if(stepping1.divide_num_s < stepping1.divide_num) {
                    stepping1.divide_num_s++;
                }
                stepping1.divide_sec = 2;
                autu_flag = 41;//�����Ƴ����ͽ�����
            } else {
                stepping1.divide_sec = 0;
                autu_flag = 9;//�����Ƴ����ͽ�����
            }
        } else {
            if(stepping1.divide_num > 0) {
                MotoSteppingSetp(2,(stepping1.divide_step*stepping1.divide_num)+(stepping1.divide_step*stepping1.divide_num_s));//��һ�ε�λ��
                //stepping1.divide_num--;
                if(stepping1.divide_num_s < stepping1.divide_num) {
                    stepping1.divide_num_s++;
                }
                stepping1.divide_sec = 2;
                autu_flag = 41;//�����Ƴ����ͽ�����
            } else {
                stepping1.divide_sec = 0;
                autu_flag = 9;//�����Ƴ����ͽ�����
            }
        }
        break;
    case 42://������� --��ʼ��ֽ
        autu_flag = 1;//ȥ��ֽ
        break;
    default:
        break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoAgeTest(void) 
* �������: void 
* ���ز���: void  
* ��    �ܣ�0 �ر��ϻ�ģʽ 1����ʼ�ϻ�ģʽ
* ��    ��: by lhb_steven
* ��    ��: 2016/8/20
************************************************************************************************************/ 
void MotoAgeTest(u8 cmd) {
    if(cmd == 0) {
        age_mode_flag = 0;
        autu_flag = 0;
        
    } else {
        age_mode_flag = 1;
        autu_flag = 1;
        MotoOff(0);
        MotoOff(1);
    }
}
/**********************************************��������***************************************************** 
* ��������: void MotoAuto(void) 
* �������: void 
* ���ز���: void  
* ��    �ܣ� 
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
************************************************************************************************************/ 
void MotoAuto(u8 cmd) {
    if(cmd == 0) {
        autu_flag = 0;
        MotoOff(0);
        MotoOff(1);
    } else if(cmd == 30) {
        autu_flag = 30;
    } else if(cmd == 40) {
        autu_flag = 40;
    } else {
        autu_flag = 1;
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
    stepping1.divide_sec = 1;//��һ�ν���
    stepping1.divide_num = num;//������Ҫ�ߵĴ���
    stepping1.arrive_setp = setp;//������Ҫ�ߵľ���
    stepping1.divide_step = setp;
}
/*
	��ʱ���жϣ�����
*/
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//����жϱ�־
    //ѹֽ���
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
                    moto1.sleep = 90;//�������� 50
                    break;
                case 2:
                    moto1.flag = 3;
                    PLATEN_SLEEP = 1;
                    moto1.sleep = moto1.sleep_set;//�������� 21
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
    //��ֽ���
    if(moto2.sleep_start == 1) {
        if(moto2.sleep_count < moto2.sleep) {
            moto2.sleep_count++;
        } else {
            moto2.sleep_count = 0;
            switch( moto2.flag ) {
                case 0:
                    moto2.flag = 1;
                    CUTTER_SLEEP = 1;
                    break;
                case 1:
                    moto2.flag = 2;
                    CUTTER_SLEEP = 0;
                    moto2.sleep = 90;//�������� 50
                    break;
                case 2:
                    moto2.flag = 3;
                    CUTTER_SLEEP = 1;
                    moto2.sleep = moto2.sleep_set;//�������� 21
                    break;
                case 3:
                    moto2.flag = 0;
                    CUTTER_SLEEP = 0;
                    moto2.sleep_start = 0;//һ�����ڽ���
                    break;
                default:
                    moto2.flag = 0;
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
                //������λ��
                if(stepping1.dir == 0) {
                    if(stepping1.step < ste_best_arrive) {
                        stepping1.step++;
                    } else {
                        stepping1.en = 0;//�رյ��
                        MotoSaveStep();//����
                        //�Զ�ģʽ �������
                        if(autu_flag == 10) {//��ֽ
                            autu_flag = 11;
                        } else if(autu_flag == 41) {//�ȷ�ģʽ
                            autu_flag = 42;
                        } else if(autu_flag == 60) {//�ϻ�ģʽ
                            autu_flag = 1;
                        }
                    }
                } else {
                    if(stepping1.step > 0) {
                        stepping1.step--;
                    }
                }
                //�ж��Ƿ���λ��
                if(stepping1.arrive_en == 1) {
                    if(stepping1.arrive_setp == stepping1.step) {
                        stepping1.arrive_en = 0;
                        stepping1.en = 0;//�رյ��
                        MotoSaveStep();//����
                        //�Զ�ģʽ �������
                        if(autu_flag == 10) {//��ֽ
                            autu_flag = 11;
                        } else if(autu_flag == 41) {//�ȷ�ģʽ
                            autu_flag = 42;
                        } else if(autu_flag == 60) {//�ϻ�ģʽ
                            autu_flag = 1;
                        }
                    }
                }
            }
            STEPPING_SLEEP = stepping1.dr;
            //���ٶ�
            //ÿ20����һ��
            if(stepping1.step_count < 10) {
                stepping1.step_count++;
            } else {
                stepping1.step_count = 0;
                if(stepping1.sleep > 1) {
                    stepping1.sleep--;
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
    if(moto1.en == 1) {
        moto1.sleep_start = 1;
    } else if(moto2.en == 1) {
        moto2.sleep_start = 1;
    }
    
    INTEN
    return;                    
}

#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void) {
    INTOFF
    moto1.yazhi_jb++;
    //PB_CR2_C22 = 0;
    INTEN
    return;                    
}

