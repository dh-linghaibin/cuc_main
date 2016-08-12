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

typedef struct DataNode
{
    u8 mode;//当前模式 0：自动模式 1：手动模式 2：差分模式  20
    u8 push_book;  //21
    u16 ste_arrive;//推书位置 22 23
} tDataNode;
/***********************************************变量声明*****************************************************
* 功    能: caidan  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
static tDataNode menu;
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 菜单参数初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/17
************************************************************************************************************/ 
void MenuInit(void) { 
    menu.mode = EepromRead(20);
    menu.push_book = EepromRead(21);
    menu.ste_arrive = EepromRead(22);
    menu.ste_arrive |= (u16)(EepromRead(23) << 8);
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
        case 0x01://步进
            if(menu.mode == 0) {
                MotoSteppingSetp(1,1);//上电复位
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
                MotoSteppingSetp(2,menu.ste_arrive);//上电复位
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
        case 0x21://切纸电机MotoAuto(0);//关闭自动模式
            ComSend(0x03,0x00, 0x00,0x00);//发送当前位置
            if(menu.mode == 0) {//自动模式
                MotoAuto(1);//关闭自动模式
            } else if(menu.mode == 1) {//手动模式
                MotoAuto(30);
                //MotoSet(1,10,0);
            } else {
                
            }
        break;
        case 0x22://切纸电机关闭
            if(menu.mode == 0) {//自动模式
                MotoAuto(0);//关闭自动模式
            } else if(menu.mode == 1) {//手动模式
                MotoAuto(0);
                //MotoOff(1);
            } else {
                
            }
        break;
        case 0x31://双向开关---向上--按下
            if(menu.mode == 0) {
                
            } else if(menu.mode == 1) {
                MotoSet(0,10,0);
            } else {
                
            }
            
        break;
        case 0x32://双向开关---向上--松开
            MotoOff(0);
        break;
        case 0x41://双向开关---向下--按下
            if(menu.mode == 0) {
                
            } else if(menu.mode == 1) {
                MotoSet(0,10,1);
            } else {
                
            }
        break;
        case 0x42://双向开关---向下--松开
        
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
            MotoSteppingSetp(2,menu.ste_arrive);
            break;
        case 0x02://压纸
            MotoSet(0,10,1);
            break;
        case 0x03://切纸
            MotoSet(1,10,0);
            break;
        case 0x04://发送当前模式
            menu.mode = ComGetDate(2);
            menu.push_book = 0;//不需要自动
            menu.ste_arrive = ComGetDate(3);
            menu.ste_arrive |= (u16)(ComGetDate(4) << 8);
            EepromWrite(20,menu.mode);
            EepromWrite(21,0x00);
            EepromWrite(22,ComGetDate(3));
            EepromWrite(23,ComGetDate(4));
            break;
        case 0x05:
            menu.mode = ComGetDate(2);
            menu.push_book = 1;//需要自动
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
/**********************************************函数定义***************************************************** 
* 函数名称: u8 MenuGetPushBook(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
u8 MenuGetPushBook(void) { 
    return menu.push_book;
}
/**********************************************函数定义***************************************************** 
* 函数名称: u16 MenuGetArrive(void) 
* 输入参数: void 
* 返回参数: u16  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
u16 MenuGetArrive(void) { 
    return menu.ste_arrive;
}












