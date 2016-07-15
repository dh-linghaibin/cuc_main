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
#ifndef __MOTO_H
#define __MOTO_H

#include "Type.h"

typedef struct {
    u8   bit0:1;
    u8   bit1:1;
    u8   bit2:1;
    u8   bit3:1;
    u8   bit4:1;
    u8   bit5:1;
    u8   bit6:1;
    u8   bit7:1;
}bitu8;

/*电机参数*/
typedef struct moto{
    u8 sleep_start;//是否开启电机
    u16 sleep_count;//速度计数
    u16 sleep;//电机速度
    u8 flag;//处于哪个环节
}moto;
/*步进电机参数*/
typedef struct stepping{
    u8 en;//使能步进电机
    u16 sleep_count;//电机速度计数
    u16 sleep;//电机速度
    u16 step;//电机位置
    u8 dir;//电机方向
    u8 dr;//脉冲方向
    u8 step_count;//加速度
}stepping;
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/13
************************************************************************************************************/ 
void MotoInit(void);

#endif


