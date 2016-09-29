/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：Menu.c
*
* 文件标识：V
* 摘    要：菜单内核
*
*
* 当前版本：V1.0
* 作    者：lhb Steven
* 完成日期：2016/3/17
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Menu.h"
#include "Led.h"
#include "Com.h"
#include "Moto.h"
#include "Eeprom.h"

/***********************************************变量声明*****************************************************
* 功    能: caidan  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
 tDataNode menu;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单参数初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
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
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetMode(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 模式状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/26
************************************************************************************************************/ 
u8 MenuGetMode(void) { 
    return menu.mode;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuModeSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 更新相应模式进入相应功能  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuModeSet(u8 cmd) {
    switch( cmd ) {
        case 0x01://步进按钮左按下
            MotoSetppingSet(1,0,0);
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x02://步进按钮松开
            MotoSetppingSet(0,0,0);
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x11://步进按钮右按下
            MotoSetppingSet(1,1,0);
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x12://步进按钮右松开
            MotoSetppingSet(0,1,0);
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x21://切纸按奶按下
            switch( menu.mode ) {
            case 0://自动模式
                MenuAutoSet(1);
                break;
            case 1://手动模式
                MotoOpen(1,1);//切纸
                break;
            case 2://等分模式
                MenuAutuSui(1);
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
            
        break;
        case 0x22://切纸电机关闭
            switch( menu.mode ) {
            case 0://自动模式
                MenuAutoSet(0);
                break;
            case 1://手动模式
                MotoOff(1);//关闭切纸纸电机
                break;
            case 2://等分模式
                MenuAutuSui(0);
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
            
        break;
        case 0x31://双向开关---向上--按下
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                MotoOpen(0,0);//压纸
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x32://双向开关---向上--松开
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                MotoOff(0);//关闭压纸纸电机
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
        break;
        case 0x41://双向开关---向下--按下
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
                break;
            case 5:
                break;
            default:
                break;
            }
            MotoReset();//刀，压纸 复位
        break;
        case 0x42://双向开关---向下--松开
            switch( menu.mode ) {
            case 0://自动模式
                break;
            case 1://手动模式
                break;
            case 2://等分模式
                break;
            case 3:
                break;
            case 4://进入老化模式
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
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAsk(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/19
************************************************************************************************************/ 
void MenuAsk(void) { 
    if(ComGetFlag() == 0x01) {
        ComSetFlag(0x00);
        switch( ComGetDate(1) ) {
        case 0x01://推书
            //MotoSteppingSetp(2,menu.ste_arrive);
            menu.ste_arrive = ComGetDate(2);
            menu.ste_arrive |= (u16)(ComGetDate(3) << 8);
            MotoSetppingSet(2,0,menu.ste_arrive);
            break;
        case 0x02://模式设置
            menu.mode = ComGetDate(2);
            EepromWrite(20,menu.mode);
            break;
        case 0x03://是否需要推书
            MotoAutuPushBook(ComGetDate(2));
            break;
        case 0x04://推纸步进电机复位
            MotoSetppingSet(3,0,0);//电机校准
            break;
        case 0x05://等分数据
            switch( ComGetDate(2) ) {
            case 0://书本长度
                menu.ste_arrive = ComGetDate(3);
                menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
                MotoDivideSet(0,menu.ste_arrive);
                break;
            case 1://裁切长度
                menu.ste_arrive = ComGetDate(3);
                menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
                MotoDivideSet(1,menu.ste_arrive);
                break;
            case 2://裁切几段
                menu.ste_arrive = ComGetDate(3);
                MotoDivideSet(2,menu.ste_arrive);
                break;
            default:
                break;
            }
            
            break;
		case 0x07://老化模式
            MotoOldMode(ComGetDate(2),ComGetDate(3));
			break;
		case 0x08://压纸电机力度设置
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












