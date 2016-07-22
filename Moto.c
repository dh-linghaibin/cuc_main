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
#include "Eeprom.h"
#include "Delay.h"
#include "Menu.h"
#include "Com.h"

#define POWER_EN     PC_ODR_ODR5 //电源控制

#define PLATEN_SLEEP PD_ODR_ODR2 //压纸机调速
#define PLATEN_DIR   PC_ODR_ODR4 //压纸机方向
#define PLATEN_HD    PB_IDR_IDR2 //压纸电机转检测
#define PLATEN_UP    PB_IDR_IDR1 //压纸电机转上限位

#define CUTTER_SLEEP PD_ODR_ODR3 //切纸机调速
#define CUTTER_DIR   PC_ODR_ODR6 //切纸机方向
#define CUTTER_UP    PB_IDR_IDR4 //切纸上限位
#define CUTTER_DOWN  PB_IDR_IDR3 //切纸下限位

#define STEPPING_DIR    PC_ODR_ODR1 //步进电机方向
#define STEPPING_SLEEP  PC_ODR_ODR2 //步进电机速度
#define STEPPING_ZERO   PB_IDR_IDR0 //步进电机零点

#define PHASE_EN    PE_CR2_C25 //交流相位使能同步

#define CUTTER_LIGHT    PC_ODR_ODR3 //刀光线使能

/*电机参数*/
moto moto1;//压纸电机
moto moto2;//切纸电机
/*步进电机参数*/
stepping stepping1;
/*自动标志位*/
static u8 autu_flag = 0;
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
    
    PC_DDR_DDR3 = 1;//刀光线使能
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    CUTTER_SLEEP = 1;
    PLATEN_SLEEP = 1;
    
    //定时器1设置，产生1ms一次的中断
	TIM1_PSCRH = 0X00;
	TIM1_PSCRL = 0Xc7;
	TIM1_ARRH = 0X00;
	TIM1_ARRL = 0X05;//0a
	TIM1_IER = 0X01;
	TIM1_CR1 = 0X01;
    
    stepping1.step = EepromRead(10);
    stepping1.step |= (u16)(EepromRead(11) << 8);
    
    //电机参数初始化
    moto1.sleep_set_group = 4;
    moto1.sleep_set = moto1.sleep_set_group*10;
    moto1.sleep = moto1.sleep_set;
    
    moto2.sleep_set_group = 4;
    moto2.sleep_set = moto2.sleep_set_group*10;
    moto2.sleep = moto1.sleep_set;
}
/**********************************************函数定义***************************************************** 
* 函数名称: static void MotoSaveStep(void) 
* 输入参数: void 
* 返回参数: static void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
static void MotoSaveStep(void) { 
    EepromWrite(10,(u8)(stepping1.step));
    EepromWrite(11,(u8)(stepping1.step >> 8));
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSet(u8 moto, u8 sleep) 
* 输入参数: u8 moto, u8 sleep 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/14
************************************************************************************************************/ 
u8 MotoSet(u8 moto, u8 sleep, u8 dir) { 
    if(moto == 0) {
        if(moto2.en == 1) {//两只电机不能同时运转
            return 0x00;
        }
        if(moto1.en == 1) {//在打开的情况下不能切换
            //关闭电机
            moto1.en = 0;
            //别忘了关闭继电器
            POWER_EN = 0;//关闭电源
            DelayMs(500);
            PLATEN_DIR = 0;//释放方向继电器
            return 0x00;
        }
        CUTTER_SLEEP = 1;
        PLATEN_SLEEP = 1;
        moto1.en = 1;//打开压纸电机
        if(dir == 0) {
            moto1.dir = 0;
            if(PLATEN_UP == 0) {
                return 0;
            }
        } else {
            moto1.dir = 1;
        }
        //压纸电机方向
        PLATEN_DIR = moto1.dir;
    } else {
        if(moto1.en == 1) {//两只电机不能同时运转
            return 0x00;
        }
        if(moto2.en == 1) {//在打开的情况下不能切换
             moto2.en = 0;
            //别忘了关闭继电器
            POWER_EN = 0;//关闭电源
            DelayMs(500);
            CUTTER_DIR = 0;//释放方向继电器
            return 0x00;
        }
        PLATEN_SLEEP = 1;
        CUTTER_SLEEP = 1;
        moto2.en = 1;//打开切纸电机
        if(dir == 0) {
            moto2.dir = 0;
            if(CUTTER_UP == 0) {
                return 0;
            }
        } else {
            moto2.dir = 1;
            if(CUTTER_DOWN == 0) {
                return 0;
            }
        }
        //切纸电机方向使能
        CUTTER_DIR = moto2.dir;
    }
    //开启中断
    PHASE_EN = 1;
    //等待继电器稳定
    DelayMs(300);
    //使能电源
    POWER_EN = 1;
    return 0;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoOff(u8 moto) 
* 输入参数: u8 moto 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/18
************************************************************************************************************/ 
void MotoOff(u8 moto) { 
    if(moto == 0) {
        //关闭电机
        moto1.en = 0;
        //别忘了关闭继电器
        POWER_EN = 0;//关闭电源
        DelayMs(500);
        PLATEN_DIR = 0;//释放方向继电器
    } else {
        moto2.en = 0;
        //别忘了关闭继电器
        POWER_EN = 0;//关闭电源
        DelayMs(500);
        CUTTER_DIR = 0;//释放方向继电器
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSetppingSet(u8 en, u8 dr,u8 set) 
* 输入参数: u8 en, u8 dr,u8 set 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MotoSetppingSet(u8 en, u8 dr,u8 set) { 
    if(en == 0) {
        stepping1.en = 0;
        MotoSaveStep();//保存
        ComSend(0x07,(u8)stepping1.step, (u8)(stepping1.step >> 8),0x00);//发送当前位置
    } else {
        if(dr == 0) {
            stepping1.dir = 0;
            STEPPING_DIR = 0;
        } else {
            stepping1.dir = 1;
            STEPPING_DIR = 1;
        }
        stepping1.sleep = 30;//设置速度
        stepping1.en = 1;//开启电机
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSteppingSetp(u8 mode, u8 arrive) 
* 输入参数: u8 mode, u8 arrive 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
void MotoSteppingSetp(u8 mode, u16 arrive) { 
    if(mode == 0) {//复位
        stepping1.dir = 1;
        STEPPING_DIR = 1;
        stepping1.sleep = 30;//设置速度
        stepping1.en = 1;//开启电机
    } else if(mode == 1) {//推书
        stepping1.dir = 0;
        STEPPING_DIR = 0;
        stepping1.sleep = 30;//设置速度
        stepping1.en = 1;//开启电机
    } else if(mode == 2) {
        if(arrive <= ste_best_arrive) {
            if(arrive == stepping1.step) {
            
            } else {
                if(arrive > stepping1.step) {
                    stepping1.dir = 0;
                    STEPPING_DIR = 0;
                } else {
                    stepping1.dir = 1;
                    STEPPING_DIR = 1;
                }
                stepping1.arrive_setp = arrive;
                stepping1.arrive_en = 1;//开启位置
                stepping1.sleep = 30;//设置速度
                stepping1.en = 1;//开启电机
            }
        } 
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoServer(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MotoServer(void) {
    //电机打开
    if(stepping1.en == 1) {
        //电机方向正传
        if(stepping1.dir == 1) {
            //回到零点
            if(STEPPING_ZERO == 0) {
                stepping1.en = 0;//关闭电机
                stepping1.step = 0;//回到零点
                MotoSaveStep();//保存
            }
        }
    }
    //压纸电机以打开
    if(moto1.en == 1) {
        if(moto1.dir == 0) {
            //压纸电机保护
            
            //到位置了
            if(PLATEN_UP == 0) {
                //关闭电机
                moto1.en = 0;
                //别忘了关闭继电器
                POWER_EN = 0;//关闭电源
                DelayMs(500);
                PLATEN_DIR = 0;//释放方向继电器
                //自动程序
                if(autu_flag == 8) {
                    autu_flag = 9;
                }
                //收到
                if(autu_flag == 22) {
                    autu_flag = 0;
                }
            }
        } else {
            static u16 hd_count = 0;
            if(PLATEN_HD == 1) {
                if(hd_count < 3000) {
                    hd_count++;
                } else {
                    hd_count = 0;
                    //压倒纸了关闭电机
                    moto1.en = 0;
                    //别忘了关闭继电器
                    POWER_EN = 0;//关闭电源
                    DelayMs(500);
                    PLATEN_DIR = 0;//释放方向继电器
                    //自动程序
                    if(autu_flag == 2) {
                        autu_flag = 3;
                    }
                }
            } else {
                hd_count = 0;
            }
        }
    }
    //切纸电机以打开
    if(moto2.en == 1) {
        if(moto2.dir == 0) {
            //切纸电机保护
            
            if(CUTTER_UP == 0) {
                //关闭电机
                moto2.en = 0;
                //别忘了关闭继电器
                POWER_EN = 0;//关闭电源
                DelayMs(500);
                CUTTER_DIR = 0;//释放方向继电器
                if(autu_flag == 6) {
                    autu_flag = 7;
                }
                //复位
                if(autu_flag == 20) {
                    autu_flag = 21;
                }
            }
        } else {
            //切纸电机保护
            
            if(CUTTER_DOWN == 0) {
                 //关闭电机
                moto2.en = 0;
                //别忘了关闭继电器
                POWER_EN = 0;//关闭电源
                DelayMs(500);
                CUTTER_DIR = 0;//释放方向继电器
                //自动程序
                if(autu_flag == 4) {
                    autu_flag = 5;
                }
            }
        }
    }
    //自动程序
    switch( autu_flag ) {
    case 1:
        //切纸上限位
        //压纸上限位
        if( (CUTTER_UP == 0) && (PLATEN_UP == 0) ){
             //开始自动程序 先压纸
            MotoSet(0,10,1);
            autu_flag = 2;
        } else {
            //先判断刀在不在位置
            if(CUTTER_UP == 1) {
                //到不在位置，回来
                MotoSet(1,10,0);
                autu_flag = 20;
            } else if(PLATEN_UP == 1) {
                //刀在位置，压纸不再位置
                MotoSet(0,10,0);
                autu_flag = 22;//结束
            }
        }
        break;
    case 2:
        break;
    case 3:
        //压纸完成，开始切纸
        MotoSet(1,10,1);
        autu_flag = 4;
        break;
    case 4:
        break;
    case 5:
        //切纸完成,把刀收回
        MotoSet(1,10,0);
        autu_flag = 6;
        break;
    case 6:
        break;
    case 7:
        //刀已经收回,收回压纸
        MotoSet(0,10,0);
        autu_flag = 8;
        break;
    case 9:
        //压纸已经收回，推书
        if(MenuGetPushBook() == 0) {
            autu_flag = 0;//结束
        } else {
            //推纸
            MotoSteppingSetp(1,0);
            autu_flag = 10;
        }
        break;
    case 11:
        //推书完成，走到位置
        MotoSteppingSetp(2,MenuGetArrive());
        autu_flag = 0;
        break;
    case 21:
        //刀已经收回，判断压纸电机
        if(PLATEN_UP == 1) {
            //刀在位置，压纸不再位置
            MotoSet(0,10,0);
            autu_flag = 22;//结束
        }
        break;
    default:
        break;
    }
    

}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoAuto(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/7/20
************************************************************************************************************/ 
void MotoAuto(u8 cmd) {
    if(cmd == 0) {
        autu_flag = 0;
        MotoOff(0);
        MotoOff(1);
    } else {
        autu_flag = 1;
    } 
}
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//清除中断标志
    //压纸电机
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
                    moto1.sleep = 90;//更新周期 50
                    break;
                case 2:
                    moto1.flag = 3;
                    PLATEN_SLEEP = 1;
                    moto1.sleep = moto1.sleep_set;//更新周期 21
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
    //切纸电机
    if(moto2.sleep_start == 1) {
        if(moto2.sleep_count < moto2.sleep) {
            moto2.sleep_count++;
        } else {
            moto2.sleep_count = 0;
            switch( moto2.flag ) {
                case 0:
                    moto2.flag = 1;
                    CUTTER_SLEEP = 1;
                    break;
                case 1:
                    moto2.flag = 2;
                    CUTTER_SLEEP = 0;
                    moto2.sleep = 90;//更新周期 50
                    break;
                case 2:
                    moto2.flag = 3;
                    CUTTER_SLEEP = 1;
                    moto2.sleep = moto2.sleep_set;//更新周期 21
                    break;
                case 3:
                    moto2.flag = 0;
                    CUTTER_SLEEP = 0;
                    moto2.sleep_start = 0;//一个周期结束
                    break;
                default:
                    moto2.flag = 0;
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
                //计算电机位置
                if(stepping1.dir == 0) {
                    if(stepping1.step < ste_best_arrive) {
                        stepping1.step++;
                    } else {
                        stepping1.en = 0;//关闭电机
                        MotoSaveStep();//保存
                        //自动模式 推书完成
                        if(autu_flag == 10) {
                            autu_flag = 11;
                        }
                    }
                } else {
                    if(stepping1.step > 0) {
                        stepping1.step--;
                    }
                }
                //判断是否到了位置
                if(stepping1.arrive_en == 1) {
                    if(stepping1.arrive_setp == stepping1.step) {
                        stepping1.arrive_en = 0;
                        stepping1.en = 0;//关闭电机
                        MotoSaveStep();//保存
                    }
                }
            }
            STEPPING_SLEEP = stepping1.dr;
            //加速度
            //每20步加一次
            if(stepping1.step_count < 10) {
                stepping1.step_count++;
            } else {
                stepping1.step_count = 0;
                if(stepping1.sleep > 1) {
                    stepping1.sleep--;
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
    if(moto1.en == 1) {
        moto1.sleep_start = 1;
    } else if(moto2.en == 1) {
        moto2.sleep_start = 1;
    }
    
    INTEN
    return;                    
}



