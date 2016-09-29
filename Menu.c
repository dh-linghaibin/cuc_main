/*******************************************************************************
* Copyright 2016 HangZhou ���ݾ���Ƽ����޹�˾
* All right reserved
*
* �ļ����ƣ�Menu.c
*
* �ļ���ʶ��V
* ժ    Ҫ���˵��ں�
*
*
* ��ǰ�汾��V1.0
* ��    �ߣ�lhb Steven
* ������ڣ�2016/3/17
* ���뻷����C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* ��ʷ��Ϣ��
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Com.h"
#include "Moto.h"
#include "Eeprom.h"

/***********************************************��������*****************************************************
* ��    ��: caidan  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
 tDataNode menu;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �˵�������ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
void MenuInit(void) {
    u8 test = 0x00;
    test = EepromRead(40);
    if(test > 100) {
        //while(1);
    } else {
        test++;
        EepromWrite(40,test);
    }
    menu.mode = EepromRead(20);
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuGetMode(void) 
* �������: void 
* ���ز���: u8  
* ��    ��: ģʽ״̬  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/26
************************************************************************************************************/ 
u8 MenuGetMode(void) { 
    return menu.mode;
}
/**********************************************��������***************************************************** 
* ��������: void MenuModeSet(u8 cmd) 
* �������: u8 cmd 
* ���ز���: void  
* ��    ��: ������Ӧģʽ������Ӧ����  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
void MenuModeSet(u8 cmd) {
    switch( cmd ) {
        case 0x01://������ť����
            MotoSetppingSet(1,0,0);
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x02://������ť�ɿ�
            MotoSetppingSet(0,0,0);
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x11://������ť�Ұ���
            MotoSetppingSet(1,1,0);
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x12://������ť���ɿ�
            MotoSetppingSet(0,1,0);
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x21://��ֽ���̰���
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                MenuAutoSet(1);
                break;
            case 1://�ֶ�ģʽ
                MotoOpen(1,1);//��ֽ
                break;
            case 2://�ȷ�ģʽ
                MenuAutuSui(1);
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
            
        break;
        case 0x22://��ֽ����ر�
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                MenuAutoSet(0);
                break;
            case 1://�ֶ�ģʽ
                MotoOff(1);//�ر���ֽֽ���
                break;
            case 2://�ȷ�ģʽ
                MenuAutuSui(0);
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
            
        break;
        case 0x31://˫�򿪹�---����--����
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                MotoOpen(0,0);//ѹֽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x32://˫�򿪹�---����--�ɿ�
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                MotoOff(0);//�ر�ѹֽֽ���
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x41://˫�򿪹�---����--����
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
            MotoReset();//����ѹֽ ��λ
        break;
        case 0x42://˫�򿪹�---����--�ɿ�
            switch( menu.mode ) {
            case 0://�Զ�ģʽ
                break;
            case 1://�ֶ�ģʽ
                break;
            case 2://�ȷ�ģʽ
                break;
            case 3:
                break;
            case 4://�����ϻ�ģʽ
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        default:
        break;
    }
}
/**********************************************��������***************************************************** 
* ��������: void MenuAsk(void) 
* �������: void 
* ���ز���: void  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/19
************************************************************************************************************/ 
void MenuAsk(void) { 
    if(ComGetFlag() == 0x01) {
        ComSetFlag(0x00);
        switch( ComGetDate(1) ) {
        case 0x01://����
            //MotoSteppingSetp(2,menu.ste_arrive);
            menu.ste_arrive = ComGetDate(2);
            menu.ste_arrive |= (u16)(ComGetDate(3) << 8);
            MotoSetppingSet(2,0,menu.ste_arrive);
            break;
        case 0x02://ģʽ����
            menu.mode = ComGetDate(2);
            EepromWrite(20,menu.mode);
            break;
        case 0x03://�Ƿ���Ҫ����
            MotoAutuPushBook(ComGetDate(2));
            break;
        case 0x04://��ֽ���������λ
            MotoSetppingSet(3,0,0);//���У׼
            break;
        case 0x05://�ȷ�����
            switch( ComGetDate(2) ) {
            case 0://�鱾����
                menu.ste_arrive = ComGetDate(3);
                menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
                MotoDivideSet(0,menu.ste_arrive);
                break;
            case 1://���г���
                menu.ste_arrive = ComGetDate(3);
                menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
                MotoDivideSet(1,menu.ste_arrive);
                break;
            case 2://���м���
                menu.ste_arrive = ComGetDate(3);
                MotoDivideSet(2,menu.ste_arrive);
                break;
            default:
                break;
            }
            
            break;
		case 0x07://�ϻ�ģʽ
            MotoOldMode(ComGetDate(2),ComGetDate(3));
			break;
		case 0x08://ѹֽ�����������
            MotoSetStrength(ComGetDate(2));
			break;
		case 0xa0:

			break;
        case 0xa1:

            break;
        case 0xb0:

        default:
            break;
        }
    }
}












