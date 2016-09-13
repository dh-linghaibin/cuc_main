/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：buntu.h
*
* 文件标识：
* 摘    要：
*
*
* 当前版本：v1.1
* 作    者：lhb Steven
* 完成日期：2016/3/14
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "buntu.h"
#include "Delay.h"
#include "Time.h"

#define BUT_OUT_PA  PA_IDR_IDR1
#define BUT_IN_PA   PA_IDR_IDR2

#define BUT_CUTTER_IN   PB_IDR_IDR6
#define BUT_CUTTER_OUT  PB_IDR_IDR7

#define BUT_PLATEN_IN   PA_IDR_IDR3
#define BUT_PLATEN_OUT  PF_IDR_IDR4

/**********************************************函数定义***************************************************** 
* 函数名称: void BuntuInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 按键初始化  
* 作    者: by lhb_steven
* 日    期: 2016/3/14
************************************************************************************************************/ 
void BuntuInit(void) { 
    PA_DDR_DDR1 = 0;//出纸-步进
    PA_CR1_C11 = 0;
    PA_CR2_C21 = 0;
    
    PA_DDR_DDR2 = 0;//进纸-步进
    PA_CR1_C12 = 0;
    PA_CR2_C22 = 0;
    
    PB_DDR_DDR6 = 0;//切纸按钮1
    PB_CR1_C16 = 0;
    PB_CR2_C26 = 0;
    
    PB_DDR_DDR7 = 0;//切纸按钮2
    PB_CR1_C17 = 0;
    PB_CR2_C27 = 0;
    
    PA_DDR_DDR3 = 0;//松开压纸
    PA_CR1_C13 = 0;
    PA_CR2_C23 = 0;
    
    PF_DDR_DDR4 = 0;//压纸
    PF_CR1_C14 = 0;
    PF_CR2_C24 = 0;
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 BuntuRead(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能: 读取按键状态  
* 作    者: by lhb_steven
* 日    期: 2016/3/14
************************************************************************************************************/ 
u8 BuntuRead(void) { 
    static u16 but_out_pa = 0;
    static u16 but_in_pa = 0;
    static u16 but_cutter = 0;
    static u16 but_platen_in = 0;
    static u16 but_platen_out = 0;
    if(BUT_OUT_PA == 0) {//出纸开关
        if(but_out_pa < 6000) {
            but_out_pa++;
            if(but_out_pa == 5999) {
                return 0x01;
            }
        }
    } else {
        if(but_out_pa > 5999) {
            but_out_pa = 0;
            return 0x02;
        }
        but_out_pa = 0;
    }
    
    if(BUT_IN_PA == 0) {//出纸开关
        if(but_in_pa < 6000) {
            but_in_pa++;
            if(but_in_pa == 5999) {
                return 0x11;
            }
        }
    } else {
        if(but_in_pa > 5999) {
            but_in_pa = 0;
            return 0x12;
        }
        but_in_pa = 0;
    }
    
    if(BUT_CUTTER_IN == 0) {//切纸
        if(BUT_CUTTER_OUT == 0) {
            if(but_cutter < 6000) {
                but_cutter++;
                if(but_cutter == 5999) {
                    return 0x21;
                }
            }
        } else {
            if(but_cutter > 5999) {
                but_cutter = 0;
                return 0x22;
            }
            but_cutter = 0;
        }
    } else {
        if(but_cutter > 5999) {
            but_cutter = 0;
            return 0x22;
        }
        but_cutter = 0;
    }
    
    if(BUT_PLATEN_IN == 0) {
        if(but_platen_in < 6000) {
            but_platen_in++;
            if(but_platen_in == 5999) {
                return 0x31;
            }
        }
    } else {
        if(but_platen_in > 5999) {
            but_platen_in = 0;
            return 0x32;
        }
        but_platen_in = 0;
    }
    
    if(BUT_PLATEN_OUT == 0) {
        if(but_platen_out < 6000) {
            but_platen_out++;
            if(but_platen_out == 5999) {
                return 0x41;
            }
        }
    } else {
        if(but_platen_out > 5999) {
            but_platen_out = 0;
            return 0x42;
        }
        but_platen_out = 0;
    }
        
    return 0x00;
}


