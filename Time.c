/*******************************************************************************
* Copyright 2016 HangZhou 杭州久天科技有限公司
* All right reserved
*
* 文件名称：time,c
*
* 文件标识：ds
* 摘    要：定时器，用于休眠
*
*
* 当前版本：v1.0
* 作    者：lhb Steven
* 完成日期：2016/3/18
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/
#include "Time.h"

void TimerInit(void) {

}

static u16 sec_flag = 0;

u16 TimerGetSec(void) {
    return sec_flag;
}

void TimerSetSec(u8 data) {
    sec_flag = data;
}




