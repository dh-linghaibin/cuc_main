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

#define SAFETY_LIGHT    PB_IDR_IDR5 //安全光幕

#define MUSIC			PD_ODR_ODR4//蜂鸣器
/*电机参数*/
moto moto1;//压纸电机
moto moto2;//切纸电机
/*步进电机参数*/
stepping stepping1;
/*自动模式结构体*/
autu autu_n;//自动结构体

music music1;//蜂鸣器
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
    PB_CR1_C12 = 1;
    PB_CR2_C22 = 1;
    
    PB_DDR_DDR1 = 0;//压纸上限位
    PB_CR1_C11 = 1;
    PB_CR2_C21 = 0;
    
    PB_DDR_DDR0 = 0;//推纸后限位--步进电机限位
    PB_CR1_C10 = 1;
    PB_CR2_C20 = 0;
    
    PB_DDR_DDR3 = 0;//切纸下限位
    PB_CR1_C13 = 0;
    PB_CR2_C23 = 1;
    
    PB_DDR_DDR4 = 0;//切纸上限位
    PB_CR1_C14 = 1;
    PB_CR2_C24 = 1;
    
    PE_DDR_DDR5 = 0;//交流相位同步
    PE_CR1_C15 = 0;
    PE_CR2_C25 = 0;//需要载打开
    
    EXTI_CR2 |= BIT(1);//开启PD口中断
	EXTI_CR2 &= ~BIT(0);
    
    PC_DDR_DDR3 = 1;//刀光线使能
    PC_CR1_C13 = 1;
    PC_CR2_C23 = 1;
    
    PB_DDR_DDR5 = 0;//安全光幕
    PB_CR1_C15 = 0;
    PB_CR2_C25 = 0;
    
    CUTTER_SLEEP = 1;
    PLATEN_SLEEP = 1;
    
    TIM1_PSCRH = 0;
    TIM1_PSCRL = 15;          //(15+1)分频为1M
    TIM1_ARRH = 0x1;
    TIM1_ARRL = 0xF4;        //每500us中断一次
    TIM1_IER = 0x01;         //允许更新中断
    TIM1_CR1 = 0x00;         //计数器使能，开始计数
    
    //TIM2/3/4/5/6须使用与芯片对应的头文件
    TIM2_PSCR_PSC = 4;          //2^4 分频为1M 
    TIM2_ARRH = 0x17;
    TIM2_ARRL = 0xA0;        //每4000us中断一次
    TIM2_IER = 0x01;         //允许更新中断
    //TIM2_CR1 = 0x00;         //计数器使能，开始计数
    
    
    CUTTER_LIGHT = 1;//打开激光指示
    
    EXTI_CR1 &= ~BIT(2);//开启PD口中断 1 0
	EXTI_CR1 |= BIT(3);
    
    EXTI_CR1_PBIS = 2;    //PB下降沿触发
    //第一次初始化
    if(EepromRead(9) != 0x55) {
        EepromWrite(9,0x55);
        EepromWrite(10,0x00);
        EepromWrite(11,0x00);
//        //力度初始化
//        EepromWrite(12,0x04);
//        EepromWrite(13,0x04);
//        
//        EepromWrite(14,0x04);
    }
    EXTI_CR2_PEIS = 1;    //上升沿触发
    
    stepping1.position = EepromRead(10);
    stepping1.position |= (u16)(EepromRead(11) << 8);
    stepping1.position_last = stepping1.position;
    
    autu_n.process = 0;
    autu_n.push_book = EepromRead(12);
    autu_n.specal_rst = EepromRead(13);
    autu_n.sui_cut_num = EepromRead(14);
    
    autu_n.sui_length = EepromRead(15);
    autu_n.sui_length |= (u16)(EepromRead(16) << 8);
    
    autu_n.sui_length = EepromRead(15);
    autu_n.sui_length |= (u16)(EepromRead(16) << 8);
    
    autu_n.sui_length_book = EepromRead(17);
    autu_n.sui_length_book |= (u16)(EepromRead(18) << 8);
    
    moto1.strength = EepromRead(19);
    moto1.strength |= (u16)(EepromRead(20) << 8);
    //保护
    if(moto1.strength > 4000) {
        moto1.strength = 1000;
    } else if(moto1.strength < 1000) {
        moto1.strength = 1000;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoError(static void) 
* 输入参数: static void 
* 返回参数: void  
* 功    能: 错误报警  
* 作    者: by lhb_steven
* 日    期: 2016/9/29
************************************************************************************************************/ 
static void MotoError(void) { 
    while(1) {
        MUSIC = 1;
        DelayMs(800);
        MUSIC = 0;
        DelayMs(800);
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSet(u8 moto, u8 sleep) 
* 输入参数: u8 moto, u8 sleep 
* 返回参数: void  
* 功    能: 打开电机
* 作    者: by lhb_steven
* 日    期: 2016/7/14
************************************************************************************************************/ 
u8 MotoOpen(u8 moto, u8 dir) { 
    //压纸电机
    if(moto == 0) {
        moto1.zero_time = 0;
        if(dir == 0) {
            //压纸
            moto1.dir = ste_dr_pla_counter;
            moto1.pla_hd_time = 0;//转速时间
        } else {
            //收刀
            moto1.dir = ste_dr_pla_positive;
        }
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//关闭接收中断使能
        }
        PHASE_EN = 1;//开启转速中断
        moto1.en = 1;//打开压纸电机
        TIM2_CR1 = 0x01;
        //压纸电机方向
        PLATEN_DIR = moto1.dir;
    } else {
        //操作切纸电机
        moto2.zero_time = 0;
        if(dir == 0) {//复位
            moto2.dir = ste_dr_cut_counter;
          
        } else {//切纸
            autu_n.specal_rst = 0;
            EepromWrite(13,autu_n.specal_rst);
            moto2.dir = ste_dr_cut_positive;
        }
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//关闭接收中断使能
        }
        moto2.en = 1;//打开切纸电机
        CUTTER_SLEEP = 0;//低电平有效
        //切纸电机方向使能
        CUTTER_DIR = moto2.dir;
    }
    //开启中断
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
        UART1_CR2_RIEN = 1;//接收中断使能
        //关闭电机
        moto1.en = 0;
        TIM2_CR1 = 0x00;//关闭定时器
        PHASE_EN = 0;//关闭中断
        DelayMs(50);//500
        PLATEN_SLEEP = 1;//拉高
        //别忘了关闭继电器
        if(moto2.en == 0) { 
            POWER_EN = 0;//关闭电源
        }
        DelayMs(50);//500
        PHASE_EN = 0;//关闭转速中断
        DelayMs(400);//500
        PLATEN_DIR = 0;//释放方向继电器
        moto1.pla_hd_time = 0;//转速
    } else if(moto == 1){
        UART1_CR2_RIEN = 1;//接收中断使能
        moto2.en = 0;
        CUTTER_SLEEP = 1;
        DelayMs(50);
        //别忘了关闭继电器
        POWER_EN = 0;//关闭电源
        DelayMs(200);//500
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
void MotoSetppingSet(u8 mode , u8 dr, u16 arrive) { 
    if(mode == 0) {//关闭电机
        UART1_CR2_RIEN = 1;//接收中断使能
        TIM1_CR1 = 0x00;//计数器使能，开始计数
        stepping1.en = 0;
        stepping1.sleep = 0;//设置速度
        EepromWrite(10,(u8)(stepping1.position));
        EepromWrite(11,(u8)(stepping1.position >> 8));
        ComSend(0x01,(u8)stepping1.position, (u8)(stepping1.position >> 8),0x00);//发送当前位置
    } else if(mode == 1){//打开电机
        if(autu_n.specal_old == 0) {
            UART1_CR2_RIEN = 0;//关闭接收中断使能
        }
        if(dr == 0) {
            stepping1.dir = 0;
            STEPPING_DIR = 0;
        } else {
            stepping1.dir = 1;
            STEPPING_DIR = 1;
        }
        stepping1.arrive_en = 0;//不走位置
        stepping1.sleep = 0;//设置速度
        stepping1.en = 1;//开启电机
        TIM1_CR1 = 0x01;//计数器使能，开始计数
    } else if(mode == 2){//走到指定位置
        if(arrive <= ste_best_arrive) {
            if(arrive >= 400) {
                if(arrive != stepping1.position) {
                    if(arrive > stepping1.position) {
                        stepping1.dir = 1;
                        STEPPING_DIR = 1;
                    } else {
                        stepping1.dir = 0;
                        STEPPING_DIR = 0;
                    }
                    if(autu_n.specal_old == 0) {
                        UART1_CR2_RIEN = 0;//关闭接收中断使能
                    }
                    stepping1.arrive_en = 1;//开启位置
                    stepping1.arrive_setp = arrive;
                    stepping1.sleep = 0;//设置速度
                    stepping1.en = 1;//开启电机
                    TIM1_CR1 = 0x01;//计数器使能，开始计数
                }
            }
        } 
    } else {//校准模式
        autu_n.process = 13;
        autu_n.en = 1;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoDivideSet(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能： 
* 作    者: by lhb_steven
* 日    期: 2016/8/20
************************************************************************************************************/ 
void MotoDivideSet(u8 num, u16 setp) {
    switch( num ) {
        case 0://书本长度
            autu_n.sui_length_book = setp;
            EepromWrite(17,(u8)(autu_n.sui_length_book));
            EepromWrite(18,(u8)(autu_n.sui_length_book >> 8));
            break;
        case 1://每段床度
            autu_n.sui_length = setp;
            EepromWrite(15,(u8)(autu_n.sui_length));
            EepromWrite(16,(u8)(autu_n.sui_length >> 8));
            break;
        case 2://裁切几段
             autu_n.sui_cut_num = setp;
             EepromWrite(14,autu_n.sui_cut_num);
            break;
        default:
            break;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MusicSet(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/8/12
************************************************************************************************************/ 
void MusicSet(void) {
	music1.open = 1;//打开蜂鸣器
	MUSIC = 1;//硬件蜂鸣器打开
}

/**********************************************函数定义***************************************************** 
* 函数名称: void MotoServer(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: 电机保护  
* 作    者: by lhb_steven
* 日    期: 2016/7/15
************************************************************************************************************/ 
void MotoServer(void) {
	//蜂鸣器服务函数
//	if(music1.open == 1) {
//		if(music1.time < 10000) {
//			music1.time++;
//		} else {
//			music1.time = 0;
//			MUSIC = 0;
//		}
//	}
    //安全光幕保护
    if(SAFETY_LIGHT == 0) {
        if(moto1.en == 1) {
            //关闭电机
            //MotoOff(0);
			//鸣叫提醒
			MusicSet();
        }
        if(moto2.en == 1) {
            //关闭电机
            //MotoOff(1);
			//鸣叫提醒
			MusicSet();
        }
    }
    //步进电机保护
    if(stepping1.en == 1) {
        //往回走的方向
        if(stepping1.dir == 1) {
            //回到零点
            if(STEPPING_ZERO == 0) {
                MotoSetppingSet(0,0,0);
                stepping1.position = 4900;
            }
        }
        if(stepping1.updata < 10000) {
            stepping1.updata++;
        } else {
            stepping1.updata = 0;
            ComSend(0x01,(u8)stepping1.position, (u8)(stepping1.position >> 8),0x00);//发送当前位置
        }
    }
    //压纸电机保护
    if(moto1.en == 1) {//切纸电机打开
        if(moto1.dir == ste_dr_pla_counter) {
            //压纸
            if(moto1.pla_hd_time < 4500) {
                moto1.pla_hd_time++;
            } else {
                moto1.pla_hd_time = 0;//关闭电机
                //关闭电机
                MotoOff(0);
            }
        } else {
            //收纸
            if(PLATEN_UP == 0) {
                //碰到上限位
                //关闭电机
                MotoOff(0);
            }
        }
        if(moto1.zero_time_l < 200) {
            moto1.zero_time_l++;
        } else {
            moto1.zero_time_l = 0;
            if(moto1.zero_time < std_pla_down_pro_time) {
                moto1.zero_time++;
            } else {
                //关闭电机
                MotoOff(0);//关闭压纸纸电机
                if(moto1.dir == ste_dr_pla_counter) {
                    ComSend(0x44,0x00,0x00,0x00);//切纸次数
                } else {
                    ComSend(0x44,0x01,0x00,0x00);//切纸次数
                }
                MotoError();
            }
        }
    }
    //切纸电机保护
    if(moto2.en == 1) {
        if(moto2.dir == ste_dr_cut_positive) {
            //切纸
            if(CUTTER_DOWN == 0) {
                if(moto2.cut_down_flag == 0) {
                    moto2.cut_down_flag = 1;
                } else if(moto2.cut_down_flag == 2){
                    moto2.cut_down_flag = 0;
                     //关闭电机
                    MotoOff(1);
                    ComSend(0x06,0x00,0x00,0x00);//切纸次数
                }
            } else {
                if(moto2.cut_down_flag == 1) {
                    moto2.cut_down_flag = 2;
                }
            }
        }
        if(moto2.zero_time_l < 200) {
            moto2.zero_time_l++;
        } else {
            moto2.zero_time_l = 0;
            if(moto2.zero_time < std_pla_up_pro_time) {
                moto2.zero_time++;
            } else {
                //关闭电机
                MotoOff(1);//关闭压纸纸电机
                if(moto2.dir == ste_dr_cut_positive) {
                    ComSend(0x44,2,0x00,0x00);//切纸次数
                } else {
                    ComSend(0x44,3,0x00,0x00);//切纸次数
                }
                MotoError();
            }
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAutuSer(void) 
* 输入参数: void 
* 返回参数: void  
* 功    能: //自动模式结构体  
* 作    者: by lhb_steven
* 日    期: 2016/9/27
************************************************************************************************************/ 
void MenuAutuSer(void) { 
    if(autu_n.en == 1) {
        switch( autu_n.process ) {
        case 0://压纸
            MotoOpen(0,0);//压纸
            autu_n.time = 0;
            autu_n.process = 1;
            break;
        case 1://等待压纸完成
            if(autu_n.time < 55000) {
                autu_n.time++;
            } else {
                autu_n.time = 0;
                autu_n.process = 3;
                //autu_n.specal_rst = 0;
                MotoOpen(1,1);//切纸
            }
            if(moto1.en == 0) {
                autu_n.time = 0;
                autu_n.process = 2;
            }
            break;
        case 2://切纸
            //autu_n.specal_rst = 0;
            autu_n.process = 3;
            MotoOpen(1,1);//切纸
            break;
        case 3://等待切纸完成
            if(moto2.en == 0) {
                autu_n.process = 4;
            }
            break;
        case 4://收回压纸
            autu_n.process = 5;
            MotoOpen(0,1);//收回    
            break;
        case 5://等待收回压纸
            if(moto1.en == 0) {
                if(autu_n.specal_old == 0) {//不是老化模式
                    if(autu_n.specal_sui == 0) {
                        if(autu_n.push_book == 0) {
                            //结束
                            autu_n.en = 0;
                            autu_n.process = 0;
                        } else {
                            //继续推纸
                            autu_n.process = 6;
                        }
                    } else {
                        //进入等分位置
                        if(autu_n.sui_cut_num_last < autu_n.sui_cut_num) {
                            autu_n.process = 17;
                        } else {
                            //结束
                            autu_n.specal_sui = 0;//等分模式结束
                            autu_n.en = 0;
                            autu_n.process = 0;
                        }
                    }
                } else if(autu_n.specal_old == 1) {//切纸老化
                    autu_n.process = 0;//继续切纸
                } else if(autu_n.specal_old == 2) {//切纸加丝杆
                    autu_n.process = 19;//丝杆
                } else {
                    //结束
                    autu_n.en = 0;
                    autu_n.process = 0;
                }
            }
            break;
        case 6://判断是否需要推纸
            //推纸位置需要定义
            autu_n.process = 7;
            stepping1.position_last = stepping1.position;
            if( (stepping1.position-200) <= 400 ) {
                MotoSetppingSet(2,0,400);//推纸
            } else {
                MotoSetppingSet(2,0,stepping1.position-200);//推纸
            }
            break;
        case 7://等待推纸完成
            if(stepping1.en == 0) {
                autu_n.process = 8;
                MotoSetppingSet(2,0,stepping1.position_last);//推纸
            }
            break;
        case 8://等待收纸完成
            if(stepping1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*---------------------复位模式----------------------------------------------*/
        case 9://收刀
            autu_n.process = 10;
            MotoOpen(1,0);
            break;
        case 10://等待收刀完成
            if(moto2.en == 0) {
                autu_n.process = 11;//收刀完成
                autu_n.specal_rst = 1;
                EepromWrite(13,autu_n.specal_rst);
            }   
            break;
        case 11://收回压纸
            autu_n.process = 12;
            MotoOpen(0,1);//收回   
            break;
        case 12://等待收回压纸完成
            if(moto1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*-------------------步进电机校准--------------------------------------------*/
        case 13://走到复位位置
            autu_n.process = 14;
            MotoSetppingSet(1,1,0);
            break;
        case 14://等待复位完成
            if(stepping1.en == 0) {
                autu_n.process = 15;
            }
            break;
        case 15://走到上次的位置
            autu_n.process = 16;
            MotoSetppingSet(2,0,stepping1.position_last);
            break;
        case 16://等待走位置完成
            if(stepping1.en == 0) {
                autu_n.en = 0;
                autu_n.process = 0;
            }
            break;
/*--------------等分模式------------------------------------------------------*/
        case 17://走到位置
            autu_n.process = 18;
            if(autu_n.sui_cut_num_last < autu_n.sui_cut_num) {
                autu_n.sui_cut_num_last++;
            }
            MotoSetppingSet(2,0,autu_n.sui_length_book - (autu_n.sui_cut_num_last*autu_n.sui_length));
            break;
        case 18://等待走位置完成
            if(stepping1.en == 0) {
                autu_n.process = 0;//开始压纸
            }
            break;
/*------------老化模式--------------------------------------------------------*/
        case 19://走到原点
            autu_n.process = 20;
            MotoSetppingSet(1,1,0);
            break;
        case 20://等待步进电机走完
            if(stepping1.en == 0) {
                autu_n.process = 21;
            }
            break;
        case 21://走到开始位置
            autu_n.process = 22;
            MotoSetppingSet(2,0,400);
            break;
        case 22://等待走完
            if(stepping1.en == 0) {
                autu_n.process = 19;
                if(autu_n.specal_old == 2) {//切纸加丝杆老化
                    autu_n.process = 0;
                } else if(autu_n.specal_old == 3) {//丝杆老化
                    autu_n.process = 19;
                } else {
                    autu_n.en = 0;
                    autu_n.process = 0;
                }
            }
            break;
        default:
            break;
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: 打开或关闭 MotoOldMode(u8 cmd,u8 mode) 
* 输入参数: u8 cmd,u8 mode 
* 返回参数: 打开或关闭  
* 功    能: 老化模式  
* 作    者: by lhb_steven
* 日    期: 2016/9/29
************************************************************************************************************/ 
void MotoOldMode(u8 cmd,u8 mode) { 
    if(cmd == 0) {
        autu_n.specal_old = 0;
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//关闭压纸电机
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//关闭压纸电机
            break;
        case 2:
            MotoOff(1);//关闭切纸纸电机
            break;
        case 3:
            MotoOff(1);//关闭切纸纸电机
            if(moto1.en == 1) {//说明压纸没完成电机就动了
                MotoOff(0);//关闭压纸电机
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//关闭压纸电机
            break;
        case 5:
            MotoOff(0);//关闭压纸电机
            break;
        case 6://判断是否需要推纸
            MotoSetppingSet(0,0,0);
            break;
        case 20://等待推纸完成
            MotoSetppingSet(0,0,0);
            break;
        case 22://等待收纸完成
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.process = 0;
        autu_n.time = 0;
        autu_n.en = 0;
    } else { 
        if(autu_n.en == 0) {
            switch( mode ) {
                case 0://切纸老化
                    autu_n.specal_old = 1;
                    autu_n.process = 0;
                    break;
                case 1://切纸加丝杆老化
                    autu_n.specal_old = 2;
                    autu_n.process = 0;
                    break;
                case 2://丝杆老化
                    autu_n.specal_old = 3;
                    autu_n.process = 19;
                    break;
                case 3:
                    
                    break;
                default:
                    break;
            }
            autu_n.en = 1;
        } else {
            MotoOldMode(0,0);
        }
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAutuSui(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 开始等分模式  
* 作    者: by lhb_steven
* 日    期: 2016/9/28
************************************************************************************************************/ 
void MenuAutuSui(u8 cmd) { 
    if(cmd == 0) {
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//关闭压纸电机
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//关闭压纸电机
            break;
        case 2:
            MotoOff(1);//关闭切纸纸电机
            break;
        case 3:
            MotoOff(1);//关闭切纸纸电机
            if(moto1.en == 1) {//说明压纸没完成电机就动了
                MotoOff(0);//关闭压纸电机
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//关闭压纸电机
            break;
        case 5:
            MotoOff(0);//关闭压纸电机
            break;
        case 17:
            MotoSetppingSet(0,0,0);
            break;
        case 18://等待走位置完成
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.time = 0;
        autu_n.en = 0;
    } else {
        if(autu_n.specal_sui == 0) {
            autu_n.process = 17;
            autu_n.sui_cut_num_last = 0;
            autu_n.specal_sui = 1;//等分模式
        }
        switch( autu_n.process ) {
        case 0:
            break;
        case 1:
            MotoOpen(0,0);//打开压纸
            break;
        case 2:
            break;
        case 3:
            if(autu_n.specal == 0) {
                MotoOpen(1,1);//打开切纸
            } else {
                //压纸没完成切纸就运行了
                autu_n.specal = 0;
                autu_n.process = 0;
            }
            break;
        case 4:
            break;
        case 5:
            MotoOpen(0,1);//收回压纸电机  
            break;
        case 17:
            break;
        case 18://等待走位置完成
            MotoSetppingSet(2,0,autu_n.sui_length_book - (autu_n.sui_cut_num_last*autu_n.sui_length));
            break;
        default:
            break;
        }
        autu_n.en = 1;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MenuAutoSet(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 设置模式  
* 作    者: by lhb_steven
* 日    期: 2016/9/27
************************************************************************************************************/ 
void MenuAutoSet(u8 cmd) { 
    if(cmd == 0) {
        switch( autu_n.process ) {
        case 0:
            MotoOff(0);//关闭压纸电机
            break;
        case 1:
            autu_n.time = 0;
            MotoOff(0);//关闭压纸电机
            break;
        case 2:
            MotoOff(1);//关闭切纸纸电机
            break;
        case 3:
            MotoOff(1);//关闭切纸纸电机
            if(moto1.en == 1) {//说明压纸没完成电机就动了
                MotoOff(0);//关闭压纸电机
                autu_n.specal = 1;
            } else {
                autu_n.specal = 0;
            }
            break;
        case 4:
            MotoOff(0);//关闭压纸电机
            break;
        case 5:
            MotoOff(0);//关闭压纸电机
            break;
        case 6://判断是否需要推纸
            MotoSetppingSet(0,0,0);
            break;
        case 7://等待推纸完成
            MotoSetppingSet(0,0,0);
            break;
        case 8://等待收纸完成
            MotoSetppingSet(0,0,0);
            break;
        default:
            break;
        }
        autu_n.time = 0;
        autu_n.en = 0;
    } else {
        autu_n.specal_sui = 0;//关闭等分模式
        switch( autu_n.process ) {
        case 0:
            break;
        case 1:
            MotoOpen(0,0);//打开压纸
            break;
        case 2:
            break;
        case 3:
            if(autu_n.specal == 0) {
                MotoOpen(1,1);//打开切纸
            } else {
                //压纸没完成切纸就运行了
                autu_n.specal = 0;
                autu_n.process = 0;
            }
            break;
        case 4:
            break;
        case 5:
            MotoOpen(0,1);//收回压纸电机  
            break;
        case 6://判断是否需要推纸
            break;
        case 7://等待推纸完成
            if( (stepping1.position-200) <= 400 ) {
                MotoSetppingSet(2,0,400);//推纸
            } else {
                MotoSetppingSet(2,0,stepping1.position-200);//推纸
            }
            break;
        case 8://等待收纸完成
            MotoSetppingSet(2,0,stepping1.position_last);//推纸
            break;
        default:
            break;
        }
        autu_n.en = 1;
    }
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoReset(void) 
* 输入参数: void
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/8/15
************************************************************************************************************/ 
void MotoReset(void) {
    if(autu_n.specal_rst == 1) {
        autu_n.process = 11;
    } else {
        autu_n.process = 9;
    }
    autu_n.en = 1;
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoAutuPushBook(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能: 是否需要自动推书设置  
* 作    者: by lhb_steven
* 日    期: 2016/9/28
************************************************************************************************************/ 
void MotoAutuPushBook(u8 cmd) { 
    autu_n.push_book = cmd;
    EepromWrite(12,autu_n.push_book);
}
/**********************************************函数定义***************************************************** 
* 函数名称: void MotoSetStrength(u8 cmd) 
* 输入参数: u8 cmd 
* 返回参数: void  
* 功    能:   
* 作    者: by lhb_steven
* 日    期: 2016/9/29
************************************************************************************************************/ 
void MotoSetStrength(u8 cmd) { 
    switch( cmd ) {
        case 1:
            moto1.strength = 4000;
            break;
        case 2:
            moto1.strength = 3000;
            break;
        case 3:
            moto1.strength = 2000;
            break;
        case 4:
            moto1.strength = 1000;
            break;
        default:
            moto1.strength = 1000;
            break;
    }
    EepromWrite(19,(u8)(moto1.strength));
    EepromWrite(20,(u8)(moto1.strength >> 8));
}
/*
	定时器中断，处理
*/
long xx = 0;
static const u16 setp_sleep[10] = {800,700,600,500,400,300,250,200,160,130};
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void) {
    INTOFF
    TIM1_SR1 &= (~0x01);//清除中断标志
    if(stepping1.dr == 0) {
        stepping1.dr = 1;
        STEPPING_SLEEP = 1;
    } else {
        stepping1.dr = 0;
        STEPPING_SLEEP = 0;
        
    }
    if(stepping1.setp < 19) {//每20步就是0.1mm
        stepping1.setp++;
    } else {
        stepping1.setp = 0;
        if(stepping1.dir == 1) {
            if(stepping1.position < 4900) {//最大距离是 490-40 = 450mm
                stepping1.position++;
            } else {
                
            }
            if(stepping1.protection < 5000) {
                stepping1.protection++;
            } else {
                //错误电机错误
                ComSend(0x44,4,0,0);
                MotoSetppingSet(0,0,0);
                MotoError();
            }
        } else {
            stepping1.protection = 0;
            if(stepping1.position > 400) {//最大距离是 490-40 = 450mm
                stepping1.position--;
            } else {
                //关闭电机
                stepping1.en = 0;
            }
        }
        //走位置
        if(stepping1.arrive_en == 1) {//开启位置
            if(stepping1.arrive_setp == stepping1.position) {
                //关闭电机
                stepping1.en = 0;
                stepping1.arrive_en = 0;
            }
        }
        //关闭电机
        if(stepping1.en == 0) {
            //TIM1_CR1 = 0x00;//计数器使能，开始计数
            MotoSetppingSet(0,0,0);
        }
    }
    //加速度启动
    if(stepping1.sleep < 9) {
        static u8 stp_time = 0;
        if(stp_time < 100) {
            stp_time++;
        } else {
            stp_time = 0;
            TIM1_ARRH = (u8)(setp_sleep[stepping1.sleep] >> 8);
            TIM1_ARRL = (u8)(setp_sleep[stepping1.sleep]);//每500us中断一次
            stepping1.sleep++;
        }
    }
    INTEN
}


#pragma   vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_OVR_UIF(void)
{
    TIM2_SR1_UIF = 0;
    //add your code here
    if(moto1.sleep_start == 1) {
        switch( moto1.flag ) {
            case 0:
                moto1.flag = 1;
                PLATEN_SLEEP = 0;
                TIM2_ARRH = 0x3;
                TIM2_ARRL = 0xE8;//每1000us中断一次
                break;
            case 1:
                PLATEN_SLEEP = 1;
                moto1.flag = 0;
                moto1.sleep_start = 0;
                break;
            default:
            
            break;
        }
    }
}


#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  
}

#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void) {
    INTOFF
    //接收到50HZ 工频信号
    if(moto1.en == 1) {
        moto1.sleep_start = 1;
        TIM2_CR1 = 0x00;//计数器使能，开始计数
        TIM2_ARRH = (u8)(moto1.strength >> 8);
        TIM2_ARRL = (u8)moto1.strength;        //每4000us中断一次
        TIM2_CR1 = 0x01;//计数器使能，开始计数
    }
    INTEN
    return;                    
}

#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void) {
    INTOFF
    if(moto1.en == 1) {//切纸电机打开
        moto1.pla_hd_time = 0;
    }
    //复位位置
    if(CUTTER_UP == 0) {
        if(moto2.en == 1) {
            if(moto2.dir == ste_dr_pla_positive) {
                //关闭电机
                MotoOff(1);
            }
        }
    }
    INTEN
    return;                    
}

