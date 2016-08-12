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
* 完成日期：2016/7/11
* 编译环境：C:\Program Files (x86)\IAR Systems\Embedded Workbench 6.5\stm8        
*
* 历史信息：
*******************************************************************************/

#include "Com.h"
#include "Delay.h"
#include "Time.h"

/*串口数据*/
static com_data com_data_one;

void ComInit(void) {
    PD_DDR &= ~BIT(6);//TX
    PD_CR1 |= BIT(6); 
    PD_CR2 &= ~BIT(6);

    PD_DDR |= BIT(5);//RX
    PD_CR1 |= BIT(5); 
    PD_CR2 |= BIT(5);

    UART1_CR1=0x00;
    UART1_CR2=0x00;
    UART1_CR3=0x00; 
    UART1_BRR2=0x02;//02 0a
    UART1_BRR1=0x68;//68 08
    UART1_CR2=0x2c;//允许接收，发送，开接收中断
}
/**********************************************函数定义***************************************************** 
* 函数名称: void ComSend(u8 cmd, u8 dat1,u8 dat2,u8 dat3) 
* 输入参数: u8 cmd, u8 dat1,u8 dat2,u8 dat3 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/6/22
************************************************************************************************************/ 
void ComSend(u8 cmd, u8 dat1,u8 dat2,u8 dat3) { 
    u8 i = 0;
    u16 cheek = 0; 
    u8 data[8];
    /**/
    data[0] = com_head_frame;
    data[1] = cmd;
    data[2] = dat1;
    data[3] = dat2;
    data[4] = dat3;
    data[7] = com_tail_frame;
    /*累加校验*/
    for(i = 1;i < 4;i++) {
       cheek += data[i];
    }
    data[5] = (u8)cheek;
    data[6] = (u8)(cheek>>8);
    DelayMs(10);
    for(i = 0;i < 8;i++) {
        while((UART1_SR & 0x80) == 0x00);
        UART1_DR = data[i];
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 ComGetFlag(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/6/22
************************************************************************************************************/ 
u8 ComGetFlag(void) { 
    return com_data_one.rs_ok;
}

/**********************************************函数定义***************************************************** 
* 函数名称: u8 ComGetDate(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/6/22
************************************************************************************************************/ 
u8 ComGetDate(u8 cmd) { 
    return com_data_one.data[cmd];
}
/**********************************************函数定义***************************************************** 
* 函数名称: void ComSetFlag(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/6/23
************************************************************************************************************/ 
void ComSetFlag(u8 cmd) { 
    com_data_one.rs_ok = 0x00;
}
/**********************************************函数定义***************************************************** 
* 函数名称: u8 ComCheck(void) 
* 输入参数: void 
* 返回参数: u8  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/6/23
************************************************************************************************************/ 
u8 ComCheck(void) { 
    u16 num = 0;
    for(u8 i = 1;i < 4;i++) {
        num += com_data_one.data[i];
    }
    if(com_data_one.data[5] == (u8)num) {
        if(com_data_one.data[6] == (u8)(num>>8)) {
            return 0x01;
        } else {
            return 0x00;
        }
    } else {
        return 0x00;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void ComAsk(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/14
************************************************************************************************************/ 
void ComAsk(void) { 
    if(com_data_one.rs_ok == 1) {
        com_data_one.rs_ok = 0;
        switch( com_data_one.data[1] ) {
            case 0:
                break;
            
            default:
                break;
        }
        

    }
}


#pragma vector=0x14
__interrupt void UART1_RX_IRQHandler(void)
{
    u8 data;
    data = UART1_DR;
    /*等待数据接受完成*/
    while((UART1_SR & 0x80) == 0x00);
    /*防止数据与头帧冲突*/
    if(com_data_one.rs_flag == 0x00) {
        if(data == com_head_frame) {
            com_data_one.rs_flag = 0x01;
        }    
    }
    /*开始结束数据*/
    if(com_data_one.rs_flag > 0x00) {
        com_data_one.data[com_data_one.rs_flag-1] = data;
        com_data_one.rs_flag++;
        if(com_data_one.rs_flag == com_size+1) {
            if(data == com_tail_frame) {
                com_data_one.rs_ok = 0x01;
                com_data_one.rs_flag = 0x00;
            } else {
                com_data_one.rs_flag = 0x00;
            }
        }
    }
    return;
}




