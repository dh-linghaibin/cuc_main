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
#include "Sys.h"
#include "Moto.h"
#include "Com.h"
#include "buntu.h"
#include "Menu.h"
#include "Eeprom.h"

int main( void ) {
    SysInit();
    EeepromInit();
    MotoInit();
    ComInit();
    BuntuInit();
    MenuInit();
    INTEN
    while(1) {
        MenuModeSet(BuntuRead());
        //���������
        MotoServer();
    }
}
