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
#include "Moto.h"

#define POWER_EN     PC_ODR_ODR5 //电源控制

#define PLATEN_SLEEP PD_ODR_ODR2 //压纸机调速
#define PLATEN_DIR   PC_ODR_ODR4 //压纸机方向

#define CUTTER_SLEEP PD_ODR_ODR3 //切纸机调速
#define CUTTER_DIR   PC_ODR_ODR6 //切纸机方向

#define STEPPING_DIR    PC_ODR_ODR1 //步进电机方向
#define STEPPING_SLEEP  PC_ODR_ODR2 //步进电机速度

#define PHASE_EN    PE_CR2_C25 //交流相位使能同步
/*电机参数*/
moto moto1;
/*步进电机参数*/
stepping stepping1;
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoInit(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/13
************************************************************************************************************/ 
void MotoInit(void) {
    PD_DDR_DDR2 = 1;//压纸机调速
    PD_CR1_C12 = 1;
    PD_CR2_C22 = 1;
    
    PD_DDR_DDR3 = 1;//切纸机调速
    PD_CR1_C13 = 1;
    PD_CR2_C23 = 1;
    
    PD_DDR_DDR4 = 1;//蜂鸣器
    PD_CR1_C14 = 1;
    PD_CR2_C24 = 1;
    
    PC_DDR_DDR4 = 1;//继电器-压纸方向
    PC_CR1_C14 = 1;
    PC_CR2_C24 = 1;
    
    PC_DDR_DDR5 = 1;//继电器-电源控制
    PC_CR1_C15 = 1;
    PC_CR2_C25 = 1;
    
    PC_DDR_DDR6 = 1;//继电器-切纸方向
    PC_CR1_C16 = 1;
    PC_CR2_C26 = 1;
    
    PC_DDR_DDR1 = 1;//步进电机-方向
    PC_CR1_C11 = 1;
    PC_CR2_C21 = 1;
    
    PC_DDR_DDR2 = 1;//步进电机-脉冲
    PC_CR1_C12 = 1;
    PC_CR2_C22 = 1;
    
    PB_DDR_DDR2 = 0;//压纸霍尔
    PB_CR1_C12 = 0;
    PB_CR2_C22 = 0;
    
    PB_DDR_DDR1 = 0;//压纸上限位
    PB_CR1_C11 = 0;
    PB_CR2_C21 = 0;
    
    PB_DDR_DDR0 = 0;//推纸后限位--步进电机限位
    PB_CR1_C10 = 0;
    PB_CR2_C20 = 0;
    
    PB_DDR_DDR3 = 0;//切纸下限位
    PB_CR1_C13 = 0;
    PB_CR2_C23 = 0;
    
    PB_DDR_DDR4 = 0;//切纸上限位
    PB_CR1_C14 = 0;
    PB_CR2_C24 = 0;
    
    PE_DDR_DDR5 = 0;//交流相位同步
    PE_CR1_C15 = 0;
    PE_CR2_C25 = 0;//需要载打开
    EXTI_CR2 |= BIT(1);//开启PD口中断
	EXTI_CR2 &= ~BIT(0);
    
    //定时器1设置，产生1ms一次的中断
	TIM1_PSCRH = 0X00;
	TIM1_PSCRL = 0Xc7;
	TIM1_ARRH = 0X00;
	TIM1_ARRL = 0X0a;
	TIM1_IER = 0X01;
	TIM1_CR1 = 0X01;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSet(u8 moto, u8 sleep) 
* 输入参数: u8 moto, u8 sleep 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/14
************************************************************************************************************/ 
void MotoSet(u8 moto, u8 sleep) { 
    PHASE_EN = 1;//使能交流相位同
    //使能方向
    //使能电源
}

#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//清除中断标志
    if(moto1.sleep_start == 1) {
        if(moto1.sleep_count < moto1.sleep) {
            moto1.sleep_count++;
        } else {
            moto1.sleep_count = 0;
            switch( moto1.flag ) {
                case 0:
                    moto1.flag = 1;
                    PLATEN_SLEEP = 1;
                    break;
                case 1:
                    moto1.flag = 2;
                    PLATEN_SLEEP = 0;
                    moto1.sleep = 50;//更新周期
                    break;
                case 2:
                    moto1.flag = 3;
                    PLATEN_SLEEP = 1;
                    moto1.sleep = 21;//更新周期
                    break;
                case 3:
                    moto1.flag = 0;
                    PLATEN_SLEEP = 0;
                    moto1.sleep_start = 0;//一个周期结束
                    break;
                default:
                    moto1.flag = 0;
                    break;
            }
        }
    }
    //步进电机
    if(stepping1.en == 1) {
        if(stepping1.sleep_count < stepping1.sleep) {
            stepping1.sleep_count++;
        } else {
            stepping1.sleep_count = 0;
            //产生脉冲
            if(stepping1.dr == 0) {
                stepping1.dr = 1;
            } else {
                stepping1.dr = 0;
            }
            STEPPING_SLEEP = stepping1.dr;
            //计算电机位置
            if(stepping1.dir == 0) {
                stepping1.step++;
            } else {
                stepping1.step--;
            }
            //加速度
            //每20步加一次
            if(stepping1.step_count < 20) {
                stepping1.step_count++;
            } else {
                stepping1.step_count = 0;
                if(stepping1.sleep > 20) {
                    stepping1.sleep++;
                }
            }
        }
    }
    
    INTEN
}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void) {
    
}


#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void) {
    INTOFF
    //接收到50HZ 工频信号
    moto1.sleep_start = 1;
    INTEN
    return;                    
}