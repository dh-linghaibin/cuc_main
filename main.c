/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：
* 作    者：lhb Steven
* 完成日期：2016/7/13
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
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
        //电机服务函数
        MotoServer();
    }
}
