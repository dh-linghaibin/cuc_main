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

typedef struct DataNode
{
    u8 mode;//��ǰģʽ 0���Զ�ģʽ 1���ֶ�ģʽ 2�����ģʽ  20
    u8 push_book;  //21
    u16 ste_arrive;//����λ�� 22 23
} tDataNode;
/***********************************************��������*****************************************************
* ��    ��: caidan  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************��������***************************************************** 
* ��������: void MenuInit(void) 
* �������: void 
* ���ز���: void  
* ��    ��: �˵�������ʼ��  
* ��    ��: by lhb_steven
* ��    ��: 2016/3/17
************************************************************************************************************/ 
void MenuInit(void) { 
    menu.mode = EepromRead(20);
    menu.push_book = EepromRead(21);
    menu.ste_arrive = EepromRead(22);
    menu.ste_arrive |= (u16)(EepromRead(23) << 8);
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
        case 0x01://����
            if(menu.mode == 0) {
                MotoSteppingSetp(1,1);//�ϵ縴λ
            } else if(menu.mode == 1) {
                MotoSetppingSet(1,0,1);
            } else {
                
            }
        break;
        case 0x02:
            if(menu.mode == 0) {

            } else if(menu.mode == 1) {
                MotoSetppingSet(0,0,1);
            } else {
                
            }
        break;
        case 0x11:
            if(menu.mode == 0) {
                MotoSteppingSetp(2,menu.ste_arrive);//�ϵ縴λ
            } else if(menu.mode == 1) {
                MotoSetppingSet(1,1,1);
            } else {
                
            }
        break;
        case 0x12:
            if(menu.mode == 0) {

            } else if(menu.mode == 1) {
                MotoSetppingSet(0,1,1);
            } else {
                
            }
        break;
        case 0x21://��ֽ���MotoAuto(0);//�ر��Զ�ģʽ
            ComSend(0x03,0x00, 0x00,0x00);//���͵�ǰλ��
            if(menu.mode == 0) {//�Զ�ģʽ
                MotoAuto(1);//�ر��Զ�ģʽ
            } else if(menu.mode == 1) {//�ֶ�ģʽ
                MotoAuto(30);
                //MotoSet(1,10,0);
            } else {
                
            }
        break;
        case 0x22://��ֽ����ر�
            if(menu.mode == 0) {//�Զ�ģʽ
                MotoAuto(0);//�ر��Զ�ģʽ
            } else if(menu.mode == 1) {//�ֶ�ģʽ
                MotoAuto(0);
                //MotoOff(1);
            } else {
                
            }
        break;
        case 0x31://˫�򿪹�---����--����
            if(menu.mode == 0) {
                
            } else if(menu.mode == 1) {
                MotoSet(0,10,0);
            } else {
                
            }
            
        break;
        case 0x32://˫�򿪹�---����--�ɿ�
            MotoOff(0);
        break;
        case 0x41://˫�򿪹�---����--����
            if(menu.mode == 0) {
                
            } else if(menu.mode == 1) {
                MotoSet(0,10,1);
            } else {
                
            }
        break;
        case 0x42://˫�򿪹�---����--�ɿ�
        
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
            MotoSteppingSetp(2,menu.ste_arrive);
            break;
        case 0x02://ѹֽ
            MotoSet(0,10,1);
            break;
        case 0x03://��ֽ
            MotoSet(1,10,0);
            break;
        case 0x04://���͵�ǰģʽ
            menu.mode = ComGetDate(2);
            menu.push_book = 0;//����Ҫ�Զ�
            menu.ste_arrive = ComGetDate(3);
            menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
            EepromWrite(20,menu.mode);
            EepromWrite(21,0x00);
            EepromWrite(22,ComGetDate(3));
            EepromWrite(23,ComGetDate(4));
            break;
        case 0x05:
            menu.mode = ComGetDate(2);
            menu.push_book = 1;//��Ҫ�Զ�
            menu.ste_arrive = ComGetDate(3);
            menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
            EepromWrite(20,menu.mode);
            EepromWrite(21,0x001);
            EepromWrite(22,ComGetDate(3));
            EepromWrite(23,ComGetDate(4));
            break;
            
        default:
            break;
        }
    }
}
/**********************************************��������***************************************************** 
* ��������: u8 MenuGetPushBook(void) 
* �������: void 
* ���ز���: u8  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
************************************************************************************************************/ 
u8 MenuGetPushBook(void) { 
    return menu.push_book;
}
/**********************************************��������***************************************************** 
* ��������: u16 MenuGetArrive(void) 
* �������: void 
* ���ز���: u16  
* ��    ��:   
* ��    ��: by lhb_steven
* ��    ��: 2016/7/20
************************************************************************************************************/ 
u16 MenuGetArrive(void) { 
    return menu.ste_arrive;
}












